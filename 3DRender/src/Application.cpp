#include "Application.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

Application::Application(int width, int height, const char* title)
	: m_mainWindow(width, height, title)
{
	if (!m_mainWindow.IsValid())
	{
		Terminate("Failed to create GLFW window");
		return;
	}
	m_device.SetCurrentWindow(m_mainWindow);
}

Application::~Application()
{
}

int Application::Run()
{
	Initialize();

	while (IsRunning())
	{
		// update time
		auto startTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> duration = std::chrono::steady_clock::now() - startTime;
		UpdateTime(duration.count());

		Update();
		Render();

		// Swap buffers and poll events at the end of the frame
		m_mainWindow.SwapBuffers();
		m_device.PollEvents();
	}

	Clearup();

	return 0;
}

void Application::Initialize()
{
}

void Application::InitializeGeometry()
{
}

void Application::InitializeShaders()
{
}

void Application::Update()
{
	if (m_mainWindow.IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		Close();
	}
}

void Application::Render()
{
}

void Application::Clearup()
{
}

bool Application::IsRunning()
{
	return m_mainWindow.IsValid() && !m_mainWindow.ShouldClose();
}

void Application::LoadAndCompileShader(Shader& shader, const char* path)
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

void Application::Terminate(const char* errorMsg)
{
	if (errorMsg)
	{
		m_errorMsg = errorMsg;
	}
	if (m_mainWindow.IsValid())
	{
		m_mainWindow.Close();
	}
}

void Application::UpdateTime(float newCurrentTime)
{
	m_deltaTime = newCurrentTime - m_currentTime;
	m_currentTime = newCurrentTime;
}
