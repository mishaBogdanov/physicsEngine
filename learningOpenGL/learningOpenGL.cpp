#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "ExperimentingWithShaders.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MyMath.h"

#include "World.h"
#include <chrono>
#include <thread>
#include "TowerOfBoxes.h"
#include "floatingCollisions.h"
#include "Simple2BoxCollision.h"
#include "RoomWorld.h"
#include "vectorPrintingClass.h"
#include "rawTesla.h"
#include "Dust2Map.h"
#include "DominoesWorld.h"

int main()
{
	using std::operator""s;
	//ExperimentingWithShaders t = ExperimentingWithShaders();
	//std::this_thread::sleep_for(6s);
	//FloatingCollisions t = FloatingCollisions();
	//Simple2BoxCollision v = Simple2BoxCollision();
	//TowerOfBoxes t = TowerOfBoxes();
	//RoomWorld t = RoomWorld();
	//RawTesla t = RawTesla();
	//Dust2Map m = Dust2Map();
	DominoesWorld d = DominoesWorld();

	//World test = World(0, 0, 0, 0, false);
	//test.setupGLFW();
	//test.addFloor(10, 100, 0, -30, 0);
	//test.addModel("BasicCube.object", 1, glm::vec3(0, 140, 0), true);

	//test.addModel("simpleBox.object", 4, glm::vec3(0, 30, 0), false);
	//test.addHitbox("simpleBox.object", 4, glm::vec3(0, 30, 0));
	//test.addModel("BasicCube.object", 10, glm::vec3(3, 180, 3), true);
	//test.addModel("BasicCube.object", 5, glm::vec3(3, 200, 3), true);
	//test.changeIsMovable(1, false);
	//glm::vec3 t = glm::vec3(1, 0, 0);
	//glm::vec3 t2 = glm::vec3(0, 0, 1);
	//test.rotateModel(0, 45, t);
	//test.rotateModel(0, 45, t2);
	//glm::vec3 myvec = test.getModel(0).getTransformation() * glm::vec4(0, - sqrt(3), 0, 1);
	//glm::vec3 myvec2 = test.getModel(0).getRawTransformation() * glm::vec4(-1, -1, 1, 1);

	//vpc::printVec(myvec2);

	//test.getModel(0);

	//test.startRenderLoop();



	return 0;


}






