#include "Mesh.h"

Mesh::Mesh(std::vector <glm::vec3>& vertices, std::vector <GLuint>& indices)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;

	vao.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	vao.LinkAtrib(VBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	// Unbind all to prevent accidentally modifying them
	translation = glm::mat4(1.0f);
	vao.Unbind();
	VBO.Unbind();
	EBO.Unbind();
	
}


void Mesh::Draw(ShaderClass& shader, Camera& camera)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	vao.Bind();
	// Take care of the camera Matrix
	camera.Matrix(shader);
	//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(translation));

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}


void Mesh::updateTransLocation(glm::mat4& given, ShaderClass & shader) {
	//shader.Activate();
	//std::cout;

	translation = given;
}
