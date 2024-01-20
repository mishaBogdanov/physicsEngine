#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderClass.h"

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 vehichleOrientation = glm::vec3(1, 0, 0);
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	int width;
	int height;
	float speed;
	float sensitivity = 100.0f;
	float xScale;
	bool firstClick = true;
	bool isInGame = true;
	double currentTime;
	glm::mat4 camMatrix;
	glm::mat4 translationMatrix;
	float previousMousePosY;
	float previousMousePosX;
	glm::vec3 modelRotation;
	bool isDriving = false;
public:
	Camera();
	Camera(int widht, int height, float gxScale, glm::vec3 gPosition);

	void setMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Inputs(GLFWwindow* window);
	void Matrix(ShaderClass& shader);
	void setView(GLFWwindow* window, glm::vec3 camPos, glm::vec3 lookTo);
	void resetMousePosition(GLFWwindow* window);
	void setVehichleCamera(glm::mat4& rotationMatrix, glm::vec3& cm, GLFWwindow* window);
	void setIsDriving(bool given);
};

