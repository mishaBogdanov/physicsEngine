#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
class VBO
{
public:
	GLuint ID;
	VBO(GLfloat* verticies, GLsizeiptr size);
	VBO(std::vector<glm::vec3> & verticies);

	void Bind();
	void Unbind();
	void Delete();
};

