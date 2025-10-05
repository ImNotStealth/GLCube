#pragma once

#include "Common.h"

class Camera
{
public:
	Camera(glm::uvec2 _viewportSize);
	Camera(glm::uvec2 _viewportSize, glm::vec3 _pos);

	void OnMouseMoved(GLFWwindow* _window, double _posX, double _posY);
	void OnMouseScrolled(GLFWwindow* _window, double _xOffset, double _yOffset);
	void ProcessInput(GLFWwindow* _window, float _deltaTime);

	void ResizeViewport(glm::uvec2 _viewportSize);

	glm::mat4 GetView();
	glm::mat4 GetProjection();

public:
	glm::vec3 cameraPos;
	float yaw = 0.0f, pitch = 0.0f;
	float fov = 90.0f;

private:
	glm::uvec2 viewportSize;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float lastMouseX = 0.0f, lastMouseY = 0.0f;
	bool firstMouse = true;
};