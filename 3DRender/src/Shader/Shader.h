#pragma once
#include "../Object.h"

#include <glad/glad.h>
#include <span>

class Shader : public Object
{
public:
	enum Type
	{
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
		GeometryShader = GL_GEOMETRY_SHADER
	};

public:
	Shader(Type type);
	~Shader();
	// Get shader source
	inline void SetSource(const char* source) { SetSource(std::span(&source, 1)); };
	void SetSource(std::span<const char*> source);

	// Compile shader
	bool Compile();

	// Check the compiler is successful or not
	bool isCompiled();

	// Get the error message in compilation phase
	void GetCompilationErrors(std::span<char> errors) const;

	void Bind() const override;

};

