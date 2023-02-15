#include "MyApp.h"
#include "src/Shader/Shader.h"
#include "src/VertexObject/VertexAttribute.h"
#include "src/Color.hpp"
#include "src/Texture/Texture.h"
#include "src/Camera/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <span>
#include <vector>
#include <array>

void processInput(GLFWwindow* window, float deltaTime);
//void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCurosrPosition(GLFWwindow* window, double xpos, double ypos);
void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset);

#define SCR_WIDTH 1400
#define SCR_HEIGHT 1024

glm::mat4 Model = glm::mat4(1.0f);
glm::mat4 View = glm::mat4(1.0f);
glm::mat4 Projection;
glm::vec3 aPos = glm::vec3(0.0f);

// camera setting
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/* Camera Attributes */
float pitch = 0.0f;
float yaw = -90.0f;
float lastX = float(SCR_WIDTH) / 2.0f;
float lastY = float(SCR_HEIGHT) / 2.0f;
bool isFirstMouse = true;
float zoom = 45.0f;

struct myVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
};

const std::array<VertexAttribute, 3> s_vertexAttr =
{
	VertexAttribute(Data::Type::Float, 3),
	VertexAttribute(Data::Type::Float, 3),
	VertexAttribute(Data::Type::Float, 2)
};




MyApp::MyApp() : Application(SCR_WIDTH, SCR_HEIGHT, "3D render")
{

}

void MyApp::Initialize()
{
	InitializeGeometry();
	InitializeShaders();
	InitializeTexture();

	// set feature enable
	//GetDevice().SetFeatureEnabled(GL_DEPTH_TEST, true);
	glEnable(GL_DEPTH_TEST);
	//GetDevice().SetWireframeEnabled();




	// set callback function
	
	GLFWwindow* window = GetMainWindow().GetInternalWindow();
	//glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCurosrPosition);
	glfwSetScrollCallback(window, mouseScrollPosition);
	
}

void MyApp::Update()
{
	//do

	Application::Update();
}

void MyApp::Render()
{
	//do
	GetDevice().Clear(Color(0.0f, 0.5f, 0.0f));

	GLFWwindow* window = GetMainWindow().GetInternalWindow();
	processInput(window, GetDeltaTime());

	m_shaderProgram.Use();

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection;
	glm::vec3 aPos = glm::vec3(0.0f);

	// model matrix
	Model = glm::mat4(1.0f);
	//Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 0.5f));
	Model = glm::translate(Model, aPos);
	Model = glm::rotate(Model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_shaderProgram.SetUnifrom("model", Model);

	// view matrix

	//m_camera.SetUp(Application::GetMainWindow(), GetDeltaTime());

	View = glm::mat4(1.0f);
	View = glm::translate(View, glm::vec3(0.0f, 0.0f, -3.0f));
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	m_shaderProgram.SetUnifrom("view", View);


	// projection matrix
	
	float fov = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	float near = 0.1f, far = 100.0f;
	Projection = glm::perspective(glm::radians(zoom), fov, near, far);
	m_shaderProgram.SetUnifrom("projection", Projection);



	m_vao.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	Application::Render();

}

void MyApp::InitializeGeometry()
{
	float vertices[] =
	{
		/*Top Position* /		/* Color */			/* TexCoords */
		-0.5f,-0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 0.5f,-0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		-0.5f,-0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		/* Bottom Position */		/* Color */			/* TexCoords */
		-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 0.5f,-0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, 0.5f,-0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, 0.5f,-0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f, 0.5f,-0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		/* Left Position */		/* Color */			/* TexCoords */
		-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,-0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		/* Right Position */		/* Color */			/* TexCoords */
		 0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 0.5f, 0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,-0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		 0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 /* Back Position */		/* Color */			/* TexCoords */
		 -0.5f, 0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		  0.5f, 0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		  0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		  0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 -0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		 -0.5f, 0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 /* Front Position */		/* Color */			/* TexCoords */
		 -0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		  0.5f,-0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		  0.5f,-0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		  0.5f,-0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 -0.5f,-0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		 -0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,

	};
	m_vbo.Bind();
	m_vao.Bind();

	m_vbo.AllocateData<float>(std::span(vertices));

	// loading the vertex data attrs
	GLsizei stride = sizeof(myVertex);
	GLint offset = 0;
	GLuint location = 0;
	for (const VertexAttribute& attribute : s_vertexAttr)
	{
		m_vao.SetAttribute(location++, attribute, offset, stride);
		offset += attribute.GetSize();
	}

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

void MyApp::InitializeTexture()
{
	Texture texture1 = { "textures/wall.jpg", false };
	texture1.Bind(0);
}

void MyApp::Clearup()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void processInput(GLFWwindow* window, float deltaTime)
{
	deltaTime = deltaTime <= 0 ? 0.0002 : deltaTime;
	float cameraSpeed = 2.5f * deltaTime;

	std::cout << deltaTime << std::endl;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_TRUE)
	{
		cameraPos += cameraUp * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_TRUE)
	{
		cameraPos -= cameraUp * cameraSpeed;
	}
}

void mouseCurosrPosition(GLFWwindow* window, double xpos, double ypos)
{
	if (isFirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		isFirstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch >= 89.0f)
		pitch = 89.0f;
	if (pitch <= -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	direction.y = std::sin(glm::radians(pitch));
	direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset)
{
	zoom -= float(yOffset);
	if (zoom >= 45.0f)
		zoom = 45.0f;
	if (zoom <= 20.0f)
		zoom = 20.0f;
}
