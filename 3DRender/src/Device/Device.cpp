#include "Device.h"
#include <GLFW/glfw3.h>

Device* Device::m_instance = nullptr;

Device::Device() : m_contextLoaded(false)
{
	m_instance = this;
	glfwInit();
}

Device::~Device()
{
	m_instance = nullptr;
	glfwTerminate();
}

void Device::SetCurrentWindow(Window& window)
{
	GLFWwindow* glfwWindow = window.GetInternalWindow();
	glfwMakeContextCurrent(glfwWindow);
	m_contextLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (m_contextLoaded)
	{
		glfwSetFramebufferSizeCallback(glfwWindow, FrameBufferResized);
	}
}

void Device::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);
}

void Device::PollEvents()
{
	glfwPollEvents();
}

void Device::Clear(bool clearColor, const Color& color, bool clearDepth, GLdouble depth, bool clearStencil, GLint stencil)
{
	GLbitfield mask = 0;
	if (clearColor)
	{
		glClearColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (clearDepth)
	{
		glClearDepth(depth);
		mask |= GL_DEPTH_BUFFER_BIT;
	}
	if (clearStencil)
	{
		glClearStencil(stencil);
		mask |= GL_STENCIL_BUFFER_BIT;
	}
	glClear(mask);
}

void Device::SetFeatureEnabled(GLenum feature, bool enabled)
{
	if (enabled)
	{
		glEnable(feature);
	}
	else
	{
		glDisable(feature);
	}
}

void Device::SetWireframeEnabled()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void Device::FrameBufferResized(GLFWwindow* window, GLsizei width, GLsizei height)
{
	if (m_instance)
	{
		m_instance->SetViewport(0, 0, width, height);
	}
}
