#include "Window.h"
#include <iostream>


Window::Window(int width, int height, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
}

void Window::GetDimensions(int& width, int& height) const
{
    glfwGetWindowSize(m_window, &width, &height);
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

Window::PressedState Window::GetKeyState(int keyCode) const
{
   
    return static_cast<PressedState>(glfwGetKey(m_window, keyCode));
}

bool Window::IsKeyPressed(int keyCode)
{
    return GetKeyState(keyCode) == PressedState::Pressed;
}

glm::vec2 Window::GetMousePosition(bool normalized) const
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    glm::vec2 mousePosition(static_cast<float>(x), static_cast<float>(y));

    if (normalized)
    {
        int width, height;
        GetDimensions(width, height);
        mousePosition.x = mousePosition.x / width * 2.0f - 1.0f;
        mousePosition.y = mousePosition.y / -height * 2.0f + 1.0f;
    }
    return mousePosition;
}

