#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "Model.h"

class CollisionBox
{
private:
	glm::vec3 Vertices[8];
	glm::vec3 normals[6];
	Model* model;
public:
};

