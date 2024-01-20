#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "Model.h"
//#include "World.h"

//class World;
class Model;
struct IntersectionModel;
//void Model::addIntersection(IntersectionModel* given);


class Hitbox
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> positionedVertices;


	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> rotatedNormals;
	std::vector<float> wallsDistance;
	std::vector<int> edges;

	glm::mat4* transformation;
	glm::mat4* rotation;
	Model* assignedModel;
	glm::vec3 cm = glm::vec3(0);
	glm::vec3 translatedCm;
	int type; //for future implementation in case I need wheels, body, etc.

	void getMaxMinFromProjectionFromCm(glm::vec3& projectVec, float& max, float& min);

public:
	Hitbox(std::vector<glm::vec3> given, std::vector<glm::vec3> gNormals, Model * model, std::vector<int> edges);
	void update(glm::mat4 *givenMatrix, glm::mat4 * givenRotation);
	std::vector<glm::vec3> getNormalVectors();
	std::vector<glm::vec3> getVertices();
	int getNormalsSize();
	void getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min);
	glm::vec3* getVec(int i);
	void addCollision(IntersectionModel model);
	Model* getAssigned();
	int getVectorsSize();
	glm::vec3* getNormal(int i);
	glm::vec3* getCm();
	void getMaxMin(int i, float givenCm, float& max, float& min);
	int getEdgeNumber();
	void getEdge(int i, glm::vec3& g1, glm::vec3& g2);
	Model* getParent();

};

