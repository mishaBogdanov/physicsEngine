#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.h"

class Camera;

class Explosion
{
private:
	float scale = 0;
	float scaleRate = 10;
	float maxScale = 2;
	glm::vec3 pos = glm::vec3(0);
	bool active = false;
	Model * gModel;
	Camera* gCam;

public:
	Explosion(Model * model, Camera* cam);
	void update(float dt);
	void enable(glm::vec3 givenPos);
	void draw();
	bool getEnabled();
};

