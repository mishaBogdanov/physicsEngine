#include "Hitbox.h"
#include "MyMath.h"
#include "World.h"
#include "Model.h"
#include "vectorPrintingClass.h"



Hitbox::Hitbox(std::vector<glm::vec3> given, std::vector<glm::vec3> gNormals, Model * model, std::vector<int> gEdges) {
	assignedModel = model;


	for (int i = 0; i < given.size(); i++) {
		cm += given[i];
	}
	cm = cm / (float)given.size();
	translatedCm = cm;
	edges = gEdges;



	positionedVertices = given;
	vertices = given;
	normals = gNormals;
	rotatedNormals = gNormals;
	for (int i = 0; i < gNormals.size(); i++) {
		float max, min;
		
		this->getMaxMinFromProjectionFromCm(gNormals[i], max, min);
		wallsDistance.push_back(max);
		wallsDistance.push_back(min);
	}

}

void Hitbox::update(glm::mat4 * givenFull, glm::mat4* givenRotation) {
	transformation = givenFull;
	for (int i = 0; i < vertices.size(); i++) {
		positionedVertices[i] = (*transformation) * glm::vec4(vertices[i], 1);
	}
	translatedCm = (*transformation) * glm::vec4(cm, 1.0f);
	//std::cout << translatedCm.x << " " << translatedCm.y << " " << translatedCm.z << "\n";

	rotation = givenRotation;
	for (int i = 0; i < normals.size(); i++) {
		rotatedNormals[i] = (*rotation) * glm::vec4(normals[i], 1);
	}

}

std::vector<glm::vec3> Hitbox::getNormalVectors() {
	return rotatedNormals;
}

std::vector<glm::vec3> Hitbox::getVertices() {
	return positionedVertices;
}


int Hitbox::getNormalsSize() {
	return normals.size();
}


void Hitbox::getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min) {
	bool isFirst = true;
	for (int i = 0; i < positionedVertices.size(); i++) {
		glm::vec3 val;
		MyMath::projection(projectVec, positionedVertices[i], val);
		float square = MyMath::getVecMultiple(projectVec, val);
		if (isFirst) {
			max = square;
			min = square;
			isFirst = false;
		}
		else if (square > max) {
			max = square;
		}
		else if (square < min) {
			min = square;
		}
	}
}

glm::vec3* Hitbox::getVec(int i) {
	return &positionedVertices[i];
}

//void Hitbox::addCollision(IntersectionModel given) {
//	(*assignedModel).addIntersection(&given);
//}

Model* Hitbox::getAssigned() {
	return assignedModel;
}

int Hitbox::getVectorsSize() {
	return vertices.size();
}

glm::vec3* Hitbox::getNormal(int i) {
	return &rotatedNormals[i];
}

glm::vec3* Hitbox::getCm() {
	return &translatedCm;
}

void Hitbox::getMaxMin(int i, float givenCm, float& max, float& min) {
	max = givenCm + wallsDistance[i*2];
	min = givenCm + wallsDistance[i*2+1];
}

int Hitbox::getEdgeNumber() {
	return edges.size() / 2;
}

void Hitbox::getEdge(int i, glm::vec3& e1, glm::vec3& e2) {
	e1 = positionedVertices[edges[i * 2]];
	e2 = positionedVertices[edges[i * 2+1]];
}


void Hitbox::getMaxMinFromProjectionFromCm(glm::vec3& projectVec, float& max, float& min) {
	bool isFirst = true;
	for (int i = 0; i < positionedVertices.size(); i++) {
		glm::vec3 val;
		glm::vec3 transfered = positionedVertices[i] - cm;
		MyMath::projection(projectVec, transfered, val);
		float square = MyMath::getVecMultiple(projectVec, val);
		if (isFirst) {
			max = square;
			min = square;
			isFirst = false;
		}
		else if (square > max) {
			max = square;
		}
		else if (square < min) {
			min = square;
		}
	}
}

Model* Hitbox::getParent() {
	return assignedModel;
}
