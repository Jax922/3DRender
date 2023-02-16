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

#include "src/imgui/imgui.h"
#include "src/imgui/imgui_impl_glfw.h"
#include "src/imgui/imgui_impl_opengl3.h"



void processInput(GLFWwindow* window, float deltaTime);
//void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCurosrPosition(GLFWwindow* window, double xpos, double ypos);
void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset);

#define SCR_WIDTH 1000
#define SCR_HEIGHT 800

// camera setting
Camera m_camera;
glm::mat4 Model = glm::mat4(1.0f);
glm::mat4 View = glm::mat4(1.0f);
glm::mat4 Projection;
glm::vec3 aPos = glm::vec3(0.0f);
bool isFirstMouse = true;
float lastX = float(SCR_WIDTH) / 2.0f;
float lastY = float(SCR_HEIGHT) / 2.0f;

// gui setting
static glm::vec3 GUI_scales = { 1.0f, 1.0f, 1.0f };
static glm::vec3 GUI_colors = {1.0f, 1.0f, 1.0f};
static bool GUI_isTexture = false;
static bool GUI_isColor = false;
static float GUI_alpha = 1.0f;
static glm::vec3 GUI_rotate = { 1.0f, 1.0f, 1.0f };
static float GUI_angle = 30.0f;
static bool GUI_mesh = false;


struct myVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
	glm::vec3 normal;
};

const std::array<VertexAttribute, 4> s_vertexAttr =
{
	VertexAttribute(Data::Type::Float, 3),
	VertexAttribute(Data::Type::Float, 3),
	VertexAttribute(Data::Type::Float, 2),
	VertexAttribute(Data::Type::Float, 3)
};




MyApp::MyApp() : Application(SCR_WIDTH, SCR_HEIGHT, "3D render")
{

}

void MyApp::Initialize()
{
	InitializeGeometry();
	InitializeShaders();
	InitializeTexture();
	setupGui();
	m_lighting.Initialize();

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


	GetDevice().SetWireframeEnabled(GUI_mesh);


	//do
	GetDevice().Clear(Color(0.2f, 0.2f, 0.2f));

	GLFWwindow* window = GetMainWindow().GetInternalWindow();
	processInput(window, GetDeltaTime());

	m_shaderProgram.Use();
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("objectColor"), GUI_colors);
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("isColor"), GUI_isColor);
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("isTexture"), GUI_isTexture);
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("alpha"), GUI_alpha);

	// lighting setting
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("lightColor"), glm::vec3(1.0f, 1.0f, 1.0f));
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("lightPos"), glm::vec3(8.0f, 5.0f, 2.0f));
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("viewPos"), m_camera.GetCameraPos());
	m_shaderProgram.SetUnifrom(m_shaderProgram.GetUniformLocation("shininess"), 32.0f);


	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection;
	glm::vec3 aPos = glm::vec3(0.0f);  

	// model matrix
	Model = glm::mat4(1.0f);
	//Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 0.5f));
	//Model = glm::translate(Model, aPos);
	//Model = glm::rotate(Model, glm::radians(30.0f) * GUI_angle, GUI_rotate);
	//Model = glm::scale(Model, GUI_scales);
	m_shaderProgram.SetUnifrom("model", Model);

	// view matrix

	View = glm::mat4(1.0f);
	View = m_camera.GetViewMatrix();
	m_shaderProgram.SetUnifrom("view", View);


	// projection matrix
	float fov = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	float near = 0.1f, far = 100.0f;
	Projection = glm::perspective(glm::radians(m_camera.GetZoom()), fov, near, far);
	m_shaderProgram.SetUnifrom("projection", Projection);

	m_vao.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);



	m_lighting.Render(View, Projection);

	renderGui();

	Application::Render();

}

void MyApp::InitializeGeometry()
{
	float vertices[] =
	{
		/* Top Position */		/* Color */			/* TexCoords */		/* Normals */
	-0.5f,-0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	 0.5f,-0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f,-0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	/* Bottom Position */		/* Color */			/* TexCoords */		/* Normals */
	-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f,-1.0f,
	 0.5f,-0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f,-1.0f,
	 0.5f, 0.5f,-0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f,-1.0f,
	 0.5f, 0.5f,-0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f,-1.0f,
	-0.5f, 0.5f,-0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f,-1.0f,
	-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f,-1.0f,
	/* Left Position */		/* Color */			/* TexCoords */	    	/* Normals */
	-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	/* Right Position */		/* Color */			/* TexCoords */	   	 /* Normals */
	 0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	 /* Back Position */		/* Color */			/* TexCoords */		/* Normals */
	-0.5f, 0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 		0.0f, 1.0f, 0.0f,
	/* Front Position */		/* Color */			/* TexCoords */		/* Normals */
	-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f,-1.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f,-1.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f,-1.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f,-1.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f,-1.0f, 0.0f,
	-0.5f,-0.5f,-0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f,-1.0f, 0.0f,

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
	Texture texture1 = { "textures/container.jpg", false };
	texture1.Bind(0);
}

void MyApp::Clearup()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	clearGui();
}

void MyApp::setupGui()
{
	// Setup Dear ImGui context
	float ImGuiWidth = 450.f;
	float ImGuiHight = 600.f;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup the Platform
	ImGui_ImplGlfw_InitForOpenGL(GetMainWindow().GetInternalWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsDark();
	ImGui::SetNextWindowSize(ImVec2(ImGuiWidth, ImGuiHight));
	
}

void MyApp::renderGui()
{
	/* Start new frame for Dear ImGui */
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
	// Render your GUI
	ImGui::Begin("Model Setting"); // Note: it's better not to use ' (quotes) inside the double quotes
	ImGui::Text("FPS= %f", ImGui::GetIO().Framerate);
	ImGui::Checkbox("Texture", &GUI_isTexture);
	ImGui::Checkbox("Color", &GUI_isColor);
	ImGui::SameLine();
	ImGui::ColorEdit3("RGB Value", glm::value_ptr(GUI_colors));
	ImGui::SliderFloat("Alpha", &GUI_alpha, 0.0f, 1.0f);
	ImGui::DragFloat3("Scale", glm::value_ptr(GUI_scales), 0.1f, 0.01f, 5.0f);
	ImGui::SliderFloat3("Rotate", glm::value_ptr(GUI_rotate), 0.01f, 1.0f);
	ImGui::SliderAngle("Angle", &GUI_angle);
	ImGui::Checkbox("Show Mesh", &GUI_mesh);
	ImGui::End();

	// Render to screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyApp::clearGui()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void processInput(GLFWwindow* window, float deltaTime)
{
	deltaTime = deltaTime <= 0 ? 0.002 : deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_TRUE) // Front
		m_camera.ProcessKeyboard(Camera::FRONT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE) // Back
		m_camera.ProcessKeyboard(Camera::BACK, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE) // Right
		m_camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE) // Left
		m_camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_TRUE) // Down
		m_camera.ProcessKeyboard(Camera::UP, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_TRUE) // Up
		m_camera.ProcessKeyboard(Camera::DOWN, deltaTime);
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
	//ImGui_ImplGlfw_MouseButtonCallback(...)
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_TRUE)
	{
		m_camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset)
{
	m_camera.ProcessMouseScroll(yOffset);
}
