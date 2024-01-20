#pragma once
#include <vector>
#include "ShaderClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "Camera.h"

class Floor
{
private:
	int num;
	float spacing;
	float height;
	float x;
	float z;
	std::vector<glm::vec3> Vertices;
	std::vector<GLuint> Indicies;
	ShaderClass shader = ShaderClass("floor.vert", "floorOutline.geom", "floor.frag");
	ShaderClass shader2 = ShaderClass("floorFill.vert", "floorFill.frag");
	VAO vao;

public:
	Floor(int gNum, float gSpacing, float gheight, float gz, float gx);
	void Draw(Camera& camera);
};

