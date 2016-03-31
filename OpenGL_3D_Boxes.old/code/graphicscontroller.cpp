
#include "graphicscontroller.h"

#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <SOIL\SOIL.h>
#include <string>
#include <iostream>

#include "debug.h"
#include "math.h"


GraphicsController::GraphicsController()
{
}

GraphicsController::~GraphicsController()
{
}

bool GraphicsController::Init(WindowInfo* windowInfo)
{
    this->windowInfo = windowInfo;

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        Debug::Assert(0, "failed to init glew");
        return false;
    }

    if (!graphicsObjects.camera.Init(glm::vec3(0.0f, 0.0f, 3.0f)))
    {
        Debug::Assert(0, "failed to init camera");
        return false;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    GLint k;
    glGetIntegerv(GL_NUM_EXTENSIONS, &k);

    for (GLint i = 0; i < k; i++)
    {
        //std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
    }

    glViewport(0, 0, (GLsizei)windowInfo->width, (GLsizei)windowInfo->height);
    //set the clearing color
    glClearColor(.2f, .3f, .3f, 1.0f);

    ///
    //shaders
    ///

    //compile individual shaders
    graphicsObjects.shader.CompileAShader(graphicsObjects.shader.GetVertexShaderID(),
                                          "../shaders/vertexshader.glsl",
                                          GL_VERTEX_SHADER);
    graphicsObjects.shader.CompileAShader(graphicsObjects.shader.GetFragmentShaderID(),
                                          "../shaders/fragmentshader.glsl",
                                          GL_FRAGMENT_SHADER);

    //attach shader source to the shader object and compile it
    graphicsObjects.shader.LinkShaders();


    //create arrays of the VAO
    glGenVertexArrays(sizeof(vertexArrayObject) / sizeof(GLuint),
                      vertexArrayObject);
    glGenBuffers(sizeof(vertexBufferObject) / sizeof(GLuint),
                 vertexBufferObject);


    BindVertexDataToVAO(defBox,
                        sizeof(defBox),
                        vertexArrayObject[0],
                        vertexBufferObject[0],
                        GL_STATIC_DRAW,
                        3,
                        2);

    ///
    //textures
    ///

    //enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //enable z buffer testing
    glEnable(GL_DEPTH_TEST);

    //set wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    LoadandBindTexture("../assets/textures/awesomeface.jpg",
                       textures[0]);
    LoadandBindTexture("../assets/textures/container.jpg",
                       textures[1]);

    return true;
}

void GraphicsController::Render(GLdouble totalTime)
{

    //clear the buffer before drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Debug::Assert(graphicsObjects.shader.GetProgramID() != 0, "programID is unexpectedly zero during render");
    graphicsObjects.shader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0].textureID);
    glUniform1i(glGetUniformLocation(graphicsObjects.shader.GetProgramID(), "aTexture[0]"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1].textureID);
    glUniform1i(glGetUniformLocation(graphicsObjects.shader.GetProgramID(), "aTexture1"), 0);

    graphicsObjects.shader.shaderVariables.viewLocation = glGetUniformLocation(graphicsObjects.shader.GetProgramID(),
            "aView");

    graphicsObjects.shader.shaderVariables.projectionLocation = glGetUniformLocation(graphicsObjects.shader.GetProgramID(),
            "aProjection");

    graphicsObjects.shader.shaderVariables.modelLocation = glGetUniformLocation(graphicsObjects.shader.GetProgramID(),
            "aModel");

    glm::mat4 view;
    view = graphicsObjects.camera.GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(graphicsObjects.camera.cameraProperties.zoom),
                                  ((float)windowInfo->width / (float)windowInfo->height),
                                  .1f,
                                  1000.0f);


    graphicsObjects.shader.BindAShaderVariableMat4(graphicsObjects.shader.shaderVariables.viewLocation,
            view);

    graphicsObjects.shader.BindAShaderVariableMat4(graphicsObjects.shader.shaderVariables.projectionLocation,
            projection);

    //draw
    glBindVertexArray(vertexArrayObject[0]);
    for (GLuint i = 0; i < 10; i++)
    {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        if (i % 2 == 0)
        {
            model = glm::rotate(model,
                                (GLfloat)totalTime * glm::radians(55.0f),
                                glm::vec3(1.0f, 0.5f, 0.5f));
        }

        graphicsObjects.shader.BindAShaderVariableMat4(graphicsObjects.shader.shaderVariables.modelLocation,
                model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
}

void GraphicsController::Quit()
{
    //de-allocate resources once they arent being used anymore
    // glDeleteVertexArrays(sizeof(vertexArrayObject) / sizeof(GLuint), vertexArrayObject);
    // glDeleteBuffers(sizeof(vertexBufferObject) / sizeof(GLuint), vertexBufferObject);
}

void GraphicsController::UpdateViewport()
{
    glViewport(0, 0, windowInfo->width, windowInfo->height);
}

void GraphicsController::BindVertexDataToVAO(const GLfloat verts[],
        GLuint vertsSize,
        GLuint& targetVertexArray,
        GLuint& targetBufferObject,
        GLenum vertexUsage,
        GLuint vertexStride,
        GLuint textureStride)
{
    glBindVertexArray(targetVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, targetBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertsSize, verts, vertexUsage);



    glVertexAttribPointer(0,
                          vertexStride,
                          GL_FLOAT,
                          GL_FALSE,
                          (vertexStride + textureStride) * sizeof(GLfloat),
                          (GLvoid*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          textureStride,
                          GL_FLOAT,
                          GL_FALSE,
                          (vertexStride + textureStride) * sizeof(GLfloat),
                          (GLvoid*)(vertexStride * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GraphicsController::LoadandBindTexture(const char* location,
        TextureInfo& textureInfo)
{

    Debug::SoftAssert(textureInfo.textureID == 0,
                      "overwriting a textureID: " + std::to_string(textureInfo.textureID));

    glGenTextures(1, &textureInfo.textureID);
    glBindTexture(GL_TEXTURE_2D, textureInfo.textureID);

    //setup texture repeat behaviour
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //setup min/mag behaviour
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load image
    //TODO texture caching
    textureInfo.imageData = SOIL_load_image(location,
                                            &textureInfo.width,
                                            &textureInfo.height,
                                            0,
                                            SOIL_LOAD_RGB);


    Debug::Assert(textureInfo.imageData != nullptr, "failed to load image" + std::string(location));

    //send image to gpu ram
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 textureInfo.width,
                 textureInfo.height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 textureInfo.imageData);

    //generate the mipmaps for the image
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(textureInfo.imageData);
    //unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}