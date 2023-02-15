#pragma once
#include "src/Application.h"
#include "src/Shader/Shader.h"
#include "src/BufferObject/VertexBufferObject.h"
#include "src/VertexObject/VertexArrayObject.h"
#include "src/Shader/ShaderProgram.h"
#include "src/Camera/Camera.h"

class MyApp : public Application
{
public:
	MyApp();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;
    void InitializeGeometry() override;
    void InitializeShaders() override;
    void InitializeTexture() override;
    void Clearup() override;

private:
    VertexBufferObject m_vbo;
    VertexArrayObject m_vao;
    ShaderProgram m_shaderProgram;
    Camera m_camera;
};

