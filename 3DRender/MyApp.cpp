#include "MyApp.h"
#include "src/Shader/Shader.h"
#include "src/VertexObject/VertexAttribute.h"
#include "src/Color.hpp"

#include <iostream>
#include <span>
#include <vector>
#include <array>


MyApp::MyApp() : Application(1024, 1024, "3D render")
{

}

void MyApp::Initialize()
{
	InitializeGeometry();
	InitializeShaders();
	
}

void MyApp::Update()
{
	//do
}

void MyApp::Render()
{
	//do
	GetDevice().Clear(Color(0.0f, 0.5f, 0.0f));

	m_shaderProgram.Use();

	m_vao.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	Application::Render();

}

void MyApp::InitializeGeometry()
{
	std::array<float, 9> vertices = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f  // bottom left
	   //-0.5f,  0.5f, 0.0f   // top left 
	};
	m_vbo.Bind();
	m_vao.Bind();

	m_vbo.AllocateData<float>(std::span(vertices));

	VertexAttribute position(Data::Type::Float, 3);
	m_vao.SetAttribute(0, position, 0);

	// Unbind VAO and VBO
	VertexArrayObject::Unbind();
	VertexBufferObject::Unbind();

}

void MyApp::InitializeShaders()
{
	Shader vertexShader(Shader::VertexShader);
	LoadAndCompileShader(vertexShader, "shaders/myApp.vert");

	Shader fragmentShader(Shader::FragmentShader);
	LoadAndCompileShader(fragmentShader, "shaders/myApp.frag");


	// Attach shaders and link
	if (!m_shaderProgram.Build(vertexShader, fragmentShader))
	{
		std::cout << "Error linking shaders" << std::endl;
	}
}
