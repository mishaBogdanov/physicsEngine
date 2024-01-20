#include "floor.h"
#include "EBO.h"

Floor::Floor(int gNum, float gSpacing, float gHeight, float gz, float gx) {
	num = gNum;
	spacing = gSpacing;
	height = gHeight;
	z = gz;
	x = gx;
	float curX = x - (float)num / 2 * spacing;
	float curZ = z - (float)num / 2 * spacing;

	for (int i = 0; i < num +1; i++) {
		for (int k = 0; k < num + 1; k++) {
			Vertices.push_back(glm::vec3(curX, height, curZ));
			curX += spacing;
		}
		curX = x - (float)num / 2 * spacing;

		curZ += spacing;
	}
	for (int i = 0; i < num * num; i++) {
		Indicies.push_back(i + i/(num));
		Indicies.push_back(i + num + 1 + i / (num));
		Indicies.push_back(i + 1 + i / (num));

		Indicies.push_back(i + num + 1 + i / (num));
		Indicies.push_back(i + num + 2 + i / (num));
		Indicies.push_back(i + 1 + i / (num));
	}
	for (int i = 0; i < Indicies.size(); i++) {
	}

	vao.Bind();

	VBO VBO(Vertices);
	EBO EBO(Indicies);

	vao.LinkAtrib(VBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);

	vao.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Floor::Draw(Camera& camera)
{
	shader.Activate();

	vao.Bind();

	camera.Matrix(shader);
	glDrawElements(GL_TRIANGLES, Indicies.size(), GL_UNSIGNED_INT, 0);

	shader2.Activate();


	camera.Matrix(shader2);

	glDrawElements(GL_TRIANGLES, Indicies.size(), GL_UNSIGNED_INT, 0);
}