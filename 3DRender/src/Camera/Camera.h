#pragma once
#include <glm/glm.hpp>
#include "../Window/Window.h"
#include <GLFW/glfw3.h>

const float PITCH = 0.0f;
const float YAW = -90.0f;
const float SENSITIVITY = 0.02f;
const float ZOOM = 45.0f;
const float SPEED = 1.0f;
//glm::vec3 DEFAULT_POS = glm::vec3(0.0f);
//glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, 3.0f);

//lastX = float(width) / 2.0f;
//lastY = float(height) / 2.0f;
//float isFirstMouse = true;





class Camera
{
public:
	enum CAMERA_MOVEMENT
	{
		UP, DOWN, LEFT, RIGHT, FRONT, BACK

	};
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = PITCH, float yaw = YAW);
	void SetUp(float width, float height);

	inline glm::vec3 GetCameraPos() { return cameraPos; };
	inline glm::vec3 GetCameraFront() { return cameraFront; };
	inline glm::vec3 GetCameraUp() { return cameraUp; };
	inline float GetZoom() { return zoom; };

	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessKeyboard(CAMERA_MOVEMENT direction, float deltaTime);
	void ProcessMouseScroll(float yoffset);

	glm::mat4 GetViewMatrix();

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;
	glm::vec3 right;
	float pitch;
	float yaw;
	bool isFirstMouse;
	float lastX, lastY;
	float sensitivity;
	float zoom;
	float movementSpeed;
	//GLFWwindow* m_window;
	//Window m_window;

private:
	void updateCameraData();
};

