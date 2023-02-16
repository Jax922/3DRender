#pragma once
#include <glm/glm.hpp>
#include "../Color.hpp"
#include "../BufferObject/VertexBufferObject.h"
#include "../VertexObject/VertexArrayObject.h"
#include "../VertexObject/VertexAttribute.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderProgram.h"

const float LightCubePos[] =
{	
	-0.5f,-0.5f, 0.5f,		
	 0.5f,-0.5f, 0.5f,		
	 0.5f, 0.5f, 0.5f,		
	 0.5f, 0.5f, 0.5f,		
	-0.5f, 0.5f, 0.5f,		
	-0.5f,-0.5f, 0.5f,		
	/* Bottom Position */	
	-0.5f,-0.5f,-0.5f,		
	 0.5f,-0.5f,-0.5f,		
	 0.5f, 0.5f,-0.5f,		
	 0.5f, 0.5f,-0.5f,		
	-0.5f, 0.5f,-0.5f,		
	-0.5f,-0.5f,-0.5f,		
	/* Left Position */		
	-0.5f,-0.5f,-0.5f,		
	-0.5f, 0.5f,-0.5f,		
	-0.5f, 0.5f, 0.5f,		
	-0.5f, 0.5f, 0.5f,		
	-0.5f,-0.5f, 0.5f,		
	-0.5f,-0.5f,-0.5f,		
	/* Right Position */	
	 0.5f,-0.5f,-0.5f,		
	 0.5f, 0.5f,-0.5f,		
	 0.5f, 0.5f, 0.5f,		
	 0.5f, 0.5f, 0.5f,		
	 0.5f,-0.5f, 0.5f,		
	 0.5f,-0.5f,-0.5f,		
	 /* Back Position */	
	 -0.5f, 0.5f,-0.5f,		
	  0.5f, 0.5f,-0.5f,		
	  0.5f, 0.5f, 0.5f,		
	  0.5f, 0.5f, 0.5f,		
	 -0.5f, 0.5f, 0.5f,		
	 -0.5f, 0.5f,-0.5f,		
	 /* Front Position */	
	 -0.5f,-0.5f,-0.5f,		
	  0.5f,-0.5f,-0.5f,		
	  0.5f,-0.5f, 0.5f,		
	  0.5f,-0.5f, 0.5f,		
	 -0.5f,-0.5f, 0.5f,		
	 -0.5f,-0.5f,-0.5f,		

};


class Lighting
{
public:
	Lighting(glm::vec3 position = glm::vec3(8.0f, 5.0f, 2.0f), Color color = Color(1.0f, 1.0f, 1.0f));
	void Initialize();
	void InitializeGeometry();
	void InitializeShaders();
	void Render(glm::mat4 view, glm::mat4 projection);

private:
	glm::vec3 m_lightingPos;
	Color m_color;

	void LoadAndCompileShader(Shader& shader, const char* path);

private: // render setting
	VertexArrayObject m_lighting_vao;
	VertexBufferObject m_lighting_vbo;
	ShaderProgram m_lighting_shaderProgram;


};

