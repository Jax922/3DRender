#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	void GetDimensions(int& width, int& height) const;
	
	// Close the window
	void Close();

	bool ShouldClose() const;

	void SwapBuffers();

	

	inline bool IsValid() const { return m_window != nullptr; };

	inline const GLFWwindow* GetInternalWindow() const { return m_window; }
	inline GLFWwindow* GetInternalWindow() { return m_window; }

public:
	enum class PressedState
	{
		Pressed = GLFW_PRESS,
		Released = GLFW_RELEASE,
		Repeated = GLFW_REPEAT,
	};
	PressedState GetKeyState(int keyCode) const;
	bool IsKeyPressed(int keyCode);

	glm::vec2 GetMousePosition(bool normalized = false) const;


private:
	GLFWwindow* m_window;
};

