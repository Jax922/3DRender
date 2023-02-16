#include "Lighting.h"
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



struct lightingVertex
{
	glm::vec3 position;
};

const std::array<VertexAttribute, 1> lightingVertexAttr =
{
	VertexAttribute(Data::Type::Float, 3)
};

Lighting::Lighting(glm::vec3 position, Color color) : m_lightingPos(position), m_color(color)
{
}

void Lighting::Initialize()
{
	InitializeGeometry();
	InitializeShaders();
}

void Lighting::InitializeGeometry()
{
	this->m_lighting_vbo.Bind();
	this->m_lighting_vao.Bind();


	this->m_lighting_vbo.AllocateData<float>(std::span(LightCubePos));

	// loading the vertex data attrs
	GLsizei l_stride = sizeof(lightingVertex);
	GLint l_offset = 0;
	GLuint l_location = 0;
	for (const VertexAttribute& attribute : lightingVertexAttr)
	{
		this->m_lighting_vao.SetAttribute(l_location++, attribute, l_offset, l_stride);
		l_offset += attribute.GetSize();
	}

	// Unbind VAO and VBO
	VertexArrayObject::Unbind();
	VertexBufferObject::Unbind();
}

void Lighting::InitializeShaders()
{
	Shader lightingVertexShader(Shader::VertexShader);
	this->LoadAndCompileShader(lightingVertexShader, "shaders/lighting.vert");

	Shader lightingFragmentShader(Shader::FragmentShader);
	this->LoadAndCompileShader(lightingFragmentShader, "shaders/lighting.frag");

	// Attach shaders and link
	if (!this->m_lighting_shaderProgram.Build(lightingVertexShader, lightingFragmentShader))
	{
		std::cout << "Error linking shaders" << std::endl;
	}
}

void Lighting::Render(glm::mat4 view, glm::mat4 projection)
{
	this->m_lighting_shaderProgram.Use();
	// model matrix
	glm::mat4 lightingModel = glm::mat4(1.0f);
	lightingModel = glm::scale(lightingModel, glm::vec3(0.1f, 0.1f, 0.1f));
	lightingModel = glm::translate(lightingModel, this->m_lightingPos);
	//lightingModel = glm::rotate(lightingModel, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	//Model = glm::scale(Model, GUI_scales);
	this->m_lighting_shaderProgram.SetUnifrom("model", lightingModel);

	// view matrix

	//m_camera.SetUp(Application::GetMainWindow(), GetDeltaTime());
	this->m_lighting_shaderProgram.SetUnifrom("view", view);


	// projection matrix
	this->m_lighting_shaderProgram.SetUnifrom("projection", projection);

	glm::vec3 lightingColor = { m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue() };
	this->m_lighting_shaderProgram.SetUnifrom(this->m_lighting_shaderProgram.GetUniformLocation("linghtingColor"), lightingColor);

	this->m_lighting_vao.Bind();
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Lighting::LoadAndCompileShader(Shader& shader, const char* path)
{
	// Open the file for reading
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "Can't find file: " << path << std::endl;
		std::cout << "Is your working directory properly set?" << std::endl;
		return;
	}

	// Dump the contents into a string
	std::stringstream stringStream;
	stringStream << file.rdbuf() << '\0';

	// Set the source code from the string
	shader.SetSource(stringStream.str().c_str());

	// Try to compile
	if (!shader.Compile())
	{
		// Get errors in case of failure
		std::array<char, 256> errors;
		shader.GetCompilationErrors(errors);
		std::cout << "Error compiling shader: " << path << std::endl;
		std::cout << errors.data() << std::endl;
	}
}