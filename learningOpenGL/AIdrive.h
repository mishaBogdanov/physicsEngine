#pragma once
#include "Model.h"
#include "vector"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class AIdrive
{
private:
	Model* givenModel; 
	std::vector<glm::vec3> checkpoints;
	int curCheckpoint;
public:
	AIdrive(Model& gModel, std::vector<glm::vec3> checkpoints);
	void update();
	void driveForward();
	void turn();
};

