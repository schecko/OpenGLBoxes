#include "shaders.h"

#include "debug.h"

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <fstream>


Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

bool Shaders::CompileAShader(GLuint& shaderID, const std::string filePath, GLuint type)
{

    shaderID = glCreateShader(type);

    Debug::Assert(shaderID != 0, "could not create shader " + std::to_string(shaderID));

    std::ifstream file(filePath);

    Debug::Assert(!file.fail(), "could not open file " + filePath);


    std::string fileContents = "";
    std::string line;

    while (std::getline(file, line))
    {
        fileContents += line + "\n";
    }

    file.close();
    const char* contentsPtr = fileContents.c_str();
    glShaderSource(shaderID, 1, &contentsPtr, nullptr);
    glCompileShader(shaderID);

    GLint success = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint maxLen = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLen);
        std::vector<char> errorLog(maxLen);
        glGetShaderInfoLog(shaderID, maxLen, &maxLen, &errorLog[0]);

        glDeleteShader(shaderID);

        Debug::Assert(0, "could not compile shader " + filePath + ", error log is: " + &errorLog[0]);
        return false;

    }

    return true;
}

void Shaders::LinkShaders()
{
    shaderInfo.programID = glCreateProgram();

    glAttachShader(shaderInfo.programID, shaderInfo.vertexShaderID);
    glAttachShader(shaderInfo.programID, shaderInfo.fragmentShaderID);

    glLinkProgram(shaderInfo.programID);

    GLint isLinked = 0;
    glGetProgramiv(shaderInfo.programID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLen = 0;
        glGetProgramiv(shaderInfo.programID, GL_INFO_LOG_LENGTH, &maxLen);

        std::vector<char> errorLog(maxLen);
        glGetProgramInfoLog(shaderInfo.programID, maxLen, &maxLen, &errorLog[0]);

//        glDeleteProgram(programID);
        glDeleteShader(shaderInfo.vertexShaderID);
        glDeleteShader(shaderInfo.fragmentShaderID);

        //place this into a reusable function
        std::printf("could not link shaders, error log is: %s", &(errorLog[0]));
    }

    glDetachShader(shaderInfo.programID, shaderInfo.vertexShaderID);
    glDetachShader(shaderInfo.programID, shaderInfo.fragmentShaderID);
    glDeleteShader(shaderInfo.vertexShaderID);
    glDeleteShader(shaderInfo.fragmentShaderID);

}

void Shaders::Use()
{
    glUseProgram(shaderInfo.programID);
}

void Shaders::BindAShaderVariableMat4(GLuint locationInShader, glm::mat4 variable)
{
    glUniformMatrix4fv(locationInShader, 1, GL_FALSE, glm::value_ptr(variable));
}

GLuint Shaders::GetProgramID()
{
    return shaderInfo.programID;
}

GLuint& Shaders::GetVertexShaderID()
{
    return shaderInfo.vertexShaderID;
}

GLuint& Shaders::GetFragmentShaderID()
{
    return shaderInfo.fragmentShaderID;
}
