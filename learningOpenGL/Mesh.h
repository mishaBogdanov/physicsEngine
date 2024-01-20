#pragma once

#include "ExperimentingWithShaders.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include <stb/stb_image.h>
#include "TextureClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <windows.h>
#include "Camera.h"
#include <vector>



class Mesh
{
public:
	std::vector <glm::vec3> vertices;
	std::vector <GLuint> indices;
	glm::vec3 pos;
	glm::mat4 translation;
	glm::vec3 facing;
	glm::vec3 velocity;
	glm::vec3 angularVelocityDirection;
	float angularVelocity;
	float mass;
	

	VAO vao;

	Mesh(std::vector <glm::vec3>& verticex, std::vector <GLuint>& indicies);

	void Draw(ShaderClass& shader, Camera& camera);
	void Draw(ShaderClass& shader, ShaderClass & shader2, Camera& camera);

	void update(float deltaT);
	void setVelocity(glm::vec3 givenV);
	void scaleVelocity(float scale);
	void updateTransLocation(glm::mat4& given, ShaderClass& shader);

};

