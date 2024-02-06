#pragma once
#include "Model.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class World;

class Helicopter
{
private:
	glm::vec3 x = glm::vec3(1,0,0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);
	Model* gModel;
	World* gWorld;
	bool shootingRight;
	float lastShotTime = 0;
	glm::vec3 propellerForce = glm::vec3(0,0,0);
	float massModdel;
public:
	Helicopter(float mass, Model * gModdel, World * givenWorld);
	void shoot();
	void processInput(float dt);
	void update(float dt);
	Model* getModdel();
};

