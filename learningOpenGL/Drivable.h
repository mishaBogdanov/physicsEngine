#pragma once
#include "Model.h"

class Drivable : public Model
{
public:
	Drivable(std::string filepath, float scale, glm::vec3 physLocation, int hitbox);
	virtual void handleInputs(float dt) = 0;
	virtual void cameraControls(Camera & cam, GLFWwindow* window) = 0;
};

