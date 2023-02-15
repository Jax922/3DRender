#pragma once
#include "src/Application.h"
#include "src/Shader/Shader.h"
#include "src/BufferObject/VertexBufferObject.h"
#include "src/VertexObject/VertexArrayObject.h"
#include "src/Shader/ShaderProgram.h"

class MyApp : public Application
{
public:
	MyApp();

protected:
    void Initialize() override;
    void Update() ;
    void Render() override;
    void InitializeGeometry() override;
    void InitializeShaders() override;

private:
    VertexBufferObject m_vbo;
    VertexArrayObject m_vao;
    ShaderProgram m_shaderProgram;
};

