#pragma once

#define GLEW_STATIC
#include <string>
#include <GLEW/glew.h>
#include <vector>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

struct ShaderInfo
{
    GLuint programID = 0;
    GLuint vertexShaderID = 0;
    GLuint fragmentShaderID = 0;

};
struct ShaderVariables
{
    GLuint viewLocation = 0;
    GLuint projectionLocation = 0;
    GLuint modelLocation = 0;
};
class Shaders
{
public:
    Shaders();
    ~Shaders();

    bool CompileAShader(GLuint& shaderID, const std::string filePath, GLuint type);

    void LinkShaders();

    void Use();
    void BindAShaderVariableMat4(GLuint locationInShader, glm::mat4 variable);


    GLuint GetProgramID();
    GLuint& GetVertexShaderID();
    GLuint& GetFragmentShaderID();

    //temp
    friend class GraphicsController;
private:
    ShaderInfo shaderInfo;
    ShaderVariables shaderVariables;


};