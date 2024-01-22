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
#include "ApacheWorld.h"

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
	//DominoesWorld d = DominoesWorld();
	ApacheWorld a = ApacheWorld();
	



	return 0;


}






