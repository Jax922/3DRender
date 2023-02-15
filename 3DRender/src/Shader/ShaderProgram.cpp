#include "ShaderProgram.h"


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
