#pragma once
#include "Model.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class World;

class Apache
{
private:
	glm::vec3 x = glm::vec3(1,0,0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);
	Model* gModel;
	World* gWorld;
	glm::vec3 propellerForce = glm::vec3(0);
	float massModdel;
public:
	Apache(float mass, Model * gModdel);
	void shoot();
	void processInput(float dt);
	void update(float dt);
	Model* getModdel();
};

