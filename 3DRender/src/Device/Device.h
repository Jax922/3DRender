#pragma once
#include <glad/glad.h>
#include "../Window/Window.h"
#include "../Color.hpp"

class Window;
struct GLFWwindow;

class Device
{
public:
	Device();
	~Device();
	void SetCurrentWindow(Window& window);
	void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	void PollEvents();
	inline void Clear(const Color& color) { Clear(true, color, false, 0.0, false, 0); }
	void Clear(bool clearColor, const Color& color, bool clearDepth, GLdouble depth, bool clearStencil, GLint stencil);
	void SetFeatureEnabled(GLenum feature, bool enabled);
	void SetWireframeEnabled(bool show);

private:
	bool m_contextLoaded; 
	static Device* m_instance;

private:
	static void FrameBufferResized(GLFWwindow* window, GLsizei width, GLsizei height);
};

