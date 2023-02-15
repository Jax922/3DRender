#include "Shader.h"

Shader::Shader(Type type) : Object(NullHandle)
{
	Handle& handle = GetHandle();
	handle = glCreateShader(type);
}

Shader::~Shader()
{
	if (IsValid())
	{
		Handle& handle = GetHandle();
		glDeleteShader(handle);
		handle = NullHandle;
	}
}

void Shader::SetSource(std::span<const char*> source)
{
	GLsizei count = static_cast<GLsizei>(source.size());
	const char** sources = source.data();
	glShaderSource(GetHandle(), count, sources, nullptr);
}

bool Shader::Compile()
{
	Handle handle = GetHandle();
	glCompileShader(handle);

	return isCompiled();
}

bool Shader::isCompiled()
{
	GLint sucess;
	Handle handle = GetHandle();
	glGetShaderiv(handle, GL_COMPILE_STATUS, &sucess);

	return sucess;
}

void Shader::GetCompilationErrors(std::span<char> errors) const
{
	Handle handle = GetHandle();
	int maxLength = static_cast<int>(errors.size());
	glGetShaderInfoLog(handle, maxLength, nullptr, errors.data());
}

void Shader::Bind() const
{
}
