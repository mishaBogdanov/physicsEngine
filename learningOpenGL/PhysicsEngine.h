#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <windows.h>
#include <vector>
#include "Mesh.h"
#include "Model.h"
#include <algorithm>
#include <set>
#include <map>



class PhysicsEngine
{
private:
	std::vector<Model*> * models;
	std::vector<IntersectionModel> currentCollisions;
	float dragConstant = 1.03;

	void generateAxis_Hitboxes(Hitbox& hitbox1, Hitbox& hitbox2, std::vector<glm::vec3>& returning);
	bool checkHitboxes(Model& model1, Model& model2, IntersectionModel& given);
	void detectCollisions();
	void dealWithCollisions();


	void dealWithFirstMovable(int i);
	void dealWithSecondMovable(int i);
	void dealWithBothMovable(int i);

	void generateContacts(Model& m1, Model& m2, std::vector<Contact>& given);
	bool checkHitboxesColliding(Hitbox& hitbox1, Hitbox& hitbox2, float& curintersect, glm::vec3& normalToIntersect);
	void comb(int N, int K, std::vector<int>& returning);

	void detectPointFace(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given);
	void detectEdgeEdge(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given);

	void correctNormalsToPointRightDirection(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given);
	glm::vec3 getClosestPointsOnLines(glm::vec3& a0, glm::vec3& b0, glm::vec3& a1, glm::vec3& b1, bool& worked);
	void addImpulses(Model& m1, Model& m2, std::vector<Contact>& givenContacts);
	void applyDrag(float dt);
	glm::vec3 calculateTVec(Model& m1, Model& m2, glm::vec3& pos, glm::vec3& normal);
	void update(float dt);
	void applyDealWithImpulses();
public:
	void updateEngine(float dt);
	void setVector(std::vector<Model* > * gModels);
	PhysicsEngine(std::vector < Model*> * models);
	PhysicsEngine();
};

