#include "Camera.h"


Camera::Camera()
{

}

void Camera::SetUp(Window window, float deltaTime)
{
	int width, height;
	window.GetDimensions(width, height);

	pitch = 0.0f;
	yaw = -90.0f;
	lastX = float(width) / 2.0f;
	lastY = float(height) / 2.0f;
	isFirstMouse = true;

	processInput(window, deltaTime);
	mouseCurosrPosition(window);
}

void Camera::processInput(Window window, float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (window.IsKeyPressed(GLFW_KEY_W))
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	if (window.IsKeyPressed(GLFW_KEY_S))
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (window.IsKeyPressed(GLFW_KEY_A))
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (window.IsKeyPressed(GLFW_KEY_D))
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (window.IsKeyPressed(GLFW_KEY_UP))
	{
		cameraPos += cameraUp * cameraSpeed;
	}
	if (window.IsKeyPressed(GLFW_KEY_DOWN))
	{
		cameraPos -= cameraUp * cameraSpeed;
	}
}


void Camera::mouseCurosrPosition(Window window)
{
	glm::vec2 pos = window.GetMousePosition();
	float xpos = pos[0];
	float ypos = pos[1];
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
