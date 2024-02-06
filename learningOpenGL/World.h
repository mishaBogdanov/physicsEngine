#pragma once
//#include "ExperimentingWithShaders.h"
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
#include "Mesh.h"
#include "Model.h"
#include "floor.h"
#include <algorithm>
#include <set>
#include "Helicopter.h"
#include "Explosion.h"
//#include "Hitbox.h"
//class Hitbox;
//class Model;

class Model;
class Hitbox;
//class Apache;

class World
{
private:
	float sizeX;
	float sizeY;
	float sizeZ;
	int divisions;

	double deltaT;
	float time;
	std::vector<Model*> models = std::vector<Model*>();
	std::vector<Floor> floors;
	std::vector<Helicopter> helicopters = std::vector<Helicopter>();
	std::vector<Explosion> explosionModels;
	
	std::vector<ShaderClass> normalShaders;
	std::vector<Model*> standardRendering;
	

	GLFWwindow* window;

	std::vector<IntersectionModel> currentCollisions;
	std::vector<int > explosions;


	static const unsigned int SCR_WIDTH = 1500;
	static const unsigned int SCR_HEIGHT = 800;
	const double xScale = 1;
	bool isConsole = true;
	HANDLE hConsole;
	DWORD dwBytesWritten;
	//wchar_t* screen = new wchar_t[SCR_WIDTH * SCR_HEIGHT + 1];
	//float pixels[SCR_WIDTH * SCR_HEIGHT * 3] = { 0 };

	Camera cam = Camera(SCR_WIDTH, SCR_HEIGHT * xScale, 1, glm::vec3(0.0f, 20.0f, 20.0f));


	bool pushed = false;
	bool isDriving = false;
	bool isFlying = false;
	Model* drivable;
	Helicopter* helicopter;
	glm::vec3 gravityAcceleration = glm::vec3(0,-98, 0);
	float dragConstant = 1.03;
	bool gravityEnabled = false;



	void generateAxis_Hitboxes(Hitbox& hitbox1, Hitbox& hitbox2, std::vector<glm::vec3>& returning);
	bool checkHitboxes(Model& model1, Model& model2, IntersectionModel & given);

	void detectCollisions();
	void dealWithCollisions();


	void dealWithExplosions();
	void applyExplosionForce(Model * missile, Model * object);
	void eraseExplosions();


	void dealWithFirstMovable(int i);
	void dealWithSecondMovable(int i);
	void generateContacts(Model& m1, Model& m2, std::vector<Contact>& given);

	void dealWithBothMovable(int i);
	bool checkHitboxesColliding(Hitbox& hitbox1, Hitbox& hitbox2, float& curintersect, glm::vec3& normalToIntersect);
	void comb(int N, int K, std::vector<int>& returning);
	void detectPointFace(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given);
	void correctNormalsToPointRightDirection(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given);
	void detectEdgeEdge(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given);
	glm::vec3 getClosestPointsOnLines(glm::vec3& a0, glm::vec3& b0, glm::vec3& a1, glm::vec3& b1, bool & worked);
	void addImpulses(Model& m1, Model& m2, std::vector<Contact>& givenContacts);
	void applyDrag(float dt);

	glm::vec3 calculateTVec(Model& m1, Model& m2, glm::vec3& pos, glm::vec3& normal);



public:
	World(float distX, float distY, float distZ, int divs, bool gravity);
	void setupGLFW();
	void update();


	void addFloor(int gNum, float gSpacing, float gHeight, float gz, float gx);
	void addModel(Model* gModel);
	void addModel(Model& given);
	void addModel(std::string given);
	void addModel(std::string given, glm::vec3 location);
	void addModel(std::string given, float scale, glm::vec3 location, bool hitbox, bool movable);
	void addModel(std::string given, float scale);
	void addModel(std::string given, float scale, bool v, glm::vec3 velocity);

	void addVehichle(std::string given, float scale, glm::vec3 location);

	void addHitbox(std::string given, float scale, glm::vec3 location);

	void rotateModel(int location, float angle, glm::vec3& norm);
	void changeIsMovable(int pos, bool value);
	void setVelocity(int index, glm::vec3 newVelocity);
	void setAngularVelocity(int index, glm::vec3 newVelocity);

	void setMass(int index, float gMass);

	void screenToPixel();
	void clearScreen();
	
	void processInput();

	void renderModels();
	void renderFLoors();
	void startRenderLoop();
	void applyDealWithImpulses();
	Model* getModel(int i);
	glm::vec3 getRelativeVelocities(Model& m1, Model& m2, glm::vec3 & atPoint, glm::vec3 & normal);
	void dealWithImpulse(Model& m1, Model& m2);
	glm::vec3* getGravity();
	int getModelsSize();
	void setTransformation(int index, glm::mat4 & gTranslation);
	void setupExplosions();
};



