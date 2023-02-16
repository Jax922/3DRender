#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef NDEBUG
ShaderProgram::Handle ShaderProgram::s_usedHandle = ShaderProgram::NullHandle;
#endif

ShaderProgram::ShaderProgram() : Object(NullHandle)
{
	Handle& handle = GetHandle();
	handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    if (IsValid())
    {
        Handle& handle = GetHandle();
        glDeleteProgram(handle);
        handle = NullHandle;
    }
}

bool ShaderProgram::Build(const Shader& vertexShader, const Shader& fragmentShader)
{
    AttachShader(vertexShader);
    AttachShader(fragmentShader);

    return Link();
}

void ShaderProgram::Use() const
{
    Handle handle = GetHandle();
    glUseProgram(handle);
#ifndef NDEBUG
    s_usedHandle = handle;
#endif
}

void ShaderProgram::AttachShader(const Shader& shader)
{
    Handle shaderProgramHandle = GetHandle();
    Handle shaderHandle = shader.GetHandle();
    glAttachShader(shaderProgramHandle, shaderHandle);
}

bool ShaderProgram::Link()
{

    Handle handle = GetHandle();

    // (todo) 02.1: Link the shader program
    glLinkProgram(handle);

    return IsLinked();
}

bool ShaderProgram::IsLinked() const
{
    GLint success;
    glGetProgramiv(GetHandle(), GL_LINK_STATUS, &success);
    return success;
}

void ShaderProgram::Bind() const
{

}

ShaderProgram::Location ShaderProgram::GetUniformLocation(const char* name)
{
    return glGetUniformLocation(GetHandle(), name);
}

void ShaderProgram::SetUnifrom(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(GetHandle(), name), value);
}

void ShaderProgram::SetUnifrom(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(GetHandle(), name), (int)value);
}

void ShaderProgram::SetUnifrom(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(GetHandle(), name), value);
}

void ShaderProgram::SetUnifrom(const char* name, glm::vec2 vector)
{
    glUniform2f(glGetUniformLocation(GetHandle(), name), vector[0], vector[1]);
}

void ShaderProgram::SetUnifrom(ShaderProgram::Location location, glm::vec3 vector)
{
    glUniform3f(location, vector[0], vector[1], vector[2]);
}

void ShaderProgram::SetUnifrom(ShaderProgram::Location location, int value)
{
    glUniform1i(location, value);
}

void ShaderProgram::SetUnifrom(ShaderProgram::Location location, bool value)
{
    glUniform1i(location, (int)value);
}

void ShaderProgram::SetUnifrom(ShaderProgram::Location location, float value)
{
    glUniform1f(location, value);
}

void ShaderProgram::SetUnifrom(const char* name, glm::vec4 vector)
{
    glUniform4f(glGetUniformLocation(GetHandle(), name), vector[0], vector[1], vector[2], vector[3]);
}

void ShaderProgram::SetUnifrom(const char* name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(GetHandle(), name), 1, GL_FALSE, glm::value_ptr(matrix));
}
