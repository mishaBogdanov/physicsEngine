#include "Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Windows.h"


Camera::Camera(int gWidth, int gHeight, float gxScale, glm::vec3 gPosition) {

	position = gPosition;
	width = gWidth;
	height = gHeight;
	xScale = gxScale;
	speed = 100;
	sensitivity = 100;
	currentTime = glfwGetTime();
}

Camera::Camera() {

}

void Camera::setMatrix(float FOVdeg, float nearPlane, float farPlane) {
	translationMatrix = glm::lookAt(position, position + Orientation, Up);
	camMatrix = glm::perspective(glm::radians(FOVdeg), (float)width / height / xScale, nearPlane, farPlane);
}


void Camera::Inputs(GLFWwindow* window) {
	double deltaT = glfwGetTime() - currentTime;
	currentTime = glfwGetTime();

	if (!isDriving) {
		if (GetKeyState('W') & 0x8000) {
			position += (float)(deltaT * speed) * Orientation;
		}
		if (GetKeyState('S') & 0x8000) {
			position -= (float)(deltaT * speed) * Orientation;
		}
		if (GetKeyState('A') & 0x8000) {
			position -= glm::normalize(glm::cross(Orientation, Up)) * (float)(deltaT * speed);
		}
		if (GetKeyState('D') & 0x8000) {
			position += glm::normalize(glm::cross(Orientation, Up)) * (float)(deltaT * speed);
		}
		if (GetKeyState(VK_SPACE) & 0x8000) {
			position += Up * (float)(deltaT * speed);
		}
		if (GetKeyState(VK_LSHIFT) & 0x8000) {
			position -= Up * (float)(deltaT * speed);
		}
	}

	
	if (isInGame)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click


		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 


		float rotX = (deltaT * sensitivity) * (float)(mouseY - previousMousePosY);
		float rotY = (deltaT * sensitivity) * (float)(mouseX - previousMousePosX);
		previousMousePosY = mouseY;
		previousMousePosX = mouseX;


		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(vehichleOrientation, glm::radians(-rotX), glm::normalize(glm::cross(vehichleOrientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			vehichleOrientation = newOrientation;
			Orientation = vehichleOrientation;
		}

		// Rotates the Orientation left and right
		vehichleOrientation = glm::rotate(vehichleOrientation, glm::radians(-rotY), Up);
		Orientation =vehichleOrientation;


		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	}resetMousePosition(window);
}

void Camera::Matrix(ShaderClass& shader) {

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camMatrix));

}



void Camera::setView(GLFWwindow * window, glm::vec3 pos, glm::vec3 lookTo) {
	position = pos;
	Orientation = glm::normalize(lookTo - pos);
	//resetMousePosition(window);
}

void Camera::resetMousePosition(GLFWwindow * window) {
	glfwSetCursorPos(window, (width / 2), (height / 4));
}

void Camera::setVehichleCamera(glm::mat4& givenVehichleRot, glm::vec3& cm, GLFWwindow* window) {
	glm::vec3 translated =  givenVehichleRot * glm::vec4(vehichleOrientation, 1.0f) ;
	setView(window, cm - translated * 70.0f, cm);
}

void Camera::setIsDriving(bool given) {
	if (given) {
		vehichleOrientation = Orientation;
	}
	else {
		vehichleOrientation = Orientation;
	}
	isDriving = given;
}

