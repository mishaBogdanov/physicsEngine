#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void myPrintVector(glm::vec3 v) {
	std::cout << v.x << " " << v.y << " " << v.z << "\n";
}