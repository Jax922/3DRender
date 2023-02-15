#pragma once
#include <glm/glm.hpp>
#include "../Window/Window.h"
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	void SetUp(Window window, float deltaTime);

	inline glm::vec3 GetCameraPos() { return cameraPos; };
	inline glm::vec3 GetCameraFront() { return cameraFront; };
	inline glm::vec3 GetCameraUp() { return cameraUp; };

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float pitch;
	float yaw;
	bool isFirstMouse = true;
	float lastX, lastY;
	float sensitivity = 0.1f;
	float zoom;
	//GLFWwindow* m_window;
	//Window m_window;

private:
	void processInput(Window window, float deltaTime);
	void mouseCurosrPosition(Window window);
};

