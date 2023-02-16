#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float pitch, float yaw)
	: cameraPos(position), worldUp(up), pitch(pitch), yaw(yaw), zoom(ZOOM), sensitivity(SENSITIVITY)
{
	//this->worldUp = up;
	this->movementSpeed = SPEED;
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->zoom = 45.0f;
	updateCameraData();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= this->sensitivity;
	yoffset *= this->sensitivity;

	this->yaw += xoffset;
	this->pitch += yoffset;

	if (this->pitch >= 89.0f)
	{
		this->pitch = 89.0f;
	}
	if (this->pitch <= -89.0f)
	{
		this->pitch = -89.0f;
	}

	updateCameraData();
}

void Camera::ProcessKeyboard(CAMERA_MOVEMENT direction, float deltaTime)
{
	const float velocity = this->movementSpeed * deltaTime;
	if (direction == FRONT)
		this->cameraPos += this->cameraFront * velocity;
	if (direction == BACK)
		this->cameraPos -= this->cameraFront * velocity;
	if (direction == RIGHT)
		this->cameraPos += this->right * velocity;
	if (direction == LEFT)
		this->cameraPos -= this->right * velocity;
	if (direction == UP)
		this->cameraPos += this->cameraUp * velocity;
	if (direction == DOWN)
		this->cameraPos -= this->cameraUp * velocity;
}

void Camera::ProcessMouseScroll(float yoffset)
{
	this->zoom -= float(yoffset);
	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
	if (this->zoom <= 20.0f)
		this->zoom = 20.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

void Camera::updateCameraData()
{
	// update cameraFront
	glm::vec3 direction;
	direction.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
	direction.y = std::sin(glm::radians(this->pitch));
	direction.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
	this->cameraFront = glm::normalize(direction);

	// update right and cameraUp
	this->right = normalize(glm::cross(this->cameraFront, this->worldUp));
	this->cameraUp = normalize(glm::cross(this->right, this->cameraFront));
}
