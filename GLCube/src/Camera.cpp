#include "Camera.h"

Camera::Camera(glm::uvec2 _viewportSize)
	: viewportSize(_viewportSize), cameraPos(glm::vec3(0.0f, 0.0f, 0.0f))
{
	lastMouseX = _viewportSize.x / 2.0f;
	lastMouseY = _viewportSize.y / 2.0f;
}

Camera::Camera(glm::uvec2 _viewportSize, glm::vec3 _pos)
	: viewportSize(_viewportSize), cameraPos(_pos)
{
	lastMouseX = _viewportSize.x / 2.0f;
	lastMouseY = _viewportSize.y / 2.0f;
}

void Camera::OnMouseMoved(GLFWwindow* _window, double _posX, double _posY)
{
	if (firstMouse)
	{
		lastMouseX = (float)_posX;
		lastMouseY = (float)_posY;
		firstMouse = false;
	}

	float xOffset = (float)_posX - lastMouseX;
	float yOffset = lastMouseY - (float)_posY;
	lastMouseX = (float)_posX;
	lastMouseY = (float)_posY;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	pitch = glm::clamp<float>(pitch, -89.f, 89.f);

	glm::vec3 direction = glm::vec3();
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::OnMouseScrolled(GLFWwindow* _window, double _xOffset, double _yOffset)
{
	fov -= (float)_yOffset;
	fov = glm::clamp<float>(fov, 20.0f, 150.0f);
}

void Camera::ProcessInput(GLFWwindow* _window, float _deltaTime)
{
	const float camSpeed = 2.5f * _deltaTime;
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraFront * camSpeed;
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraFront * camSpeed;
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
}

void Camera::ResizeViewport(glm::uvec2 _viewportSize)
{
	viewportSize = _viewportSize;
}

glm::mat4 Camera::GetView()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetProjection()
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)viewportSize.x / (float)viewportSize.y, 0.1f, 100.0f);

	return projection;
}
