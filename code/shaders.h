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
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

};
struct ShaderVariables
{
    GLuint viewLocation;
    GLuint projectionLocation;
    GLuint modelLocation;
    GLuint timeLocation;
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
    void BindAShaderVariableFloat(GLuint locationInShader, GLfloat variable);


    GLuint GetProgramID();
    GLuint& GetVertexShaderID();
    GLuint& GetFragmentShaderID();

    //temp
    friend class GraphicsController;
private:
    ShaderInfo shaderInfo;
    ShaderVariables shaderVariables;


};