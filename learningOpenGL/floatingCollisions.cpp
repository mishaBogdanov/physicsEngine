#include "floatingCollisions.h"

FloatingCollisions::FloatingCollisions() {
	World test = World(0, 0, 0, 0, false);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	test.addModel("BasicCube.object", 9, glm::vec3(0, 9 + 30, 0), true, true);
	test.addModel("BasicCube.object", 3, glm::vec3(500, 2 + 30, 9), true, true);
	test.addModel("BasicCube.object", 15, glm::vec3(-100, 2 + 30, 9), true, true);
	test.addModel("TeslaTruck.object", 20, glm::vec3(0, 2 + 30, -300), true, true);
	//test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
	//test.setVelocity(0, glm::vec3(0, 0, 40));
	test.setVelocity(1, glm::vec3(-300, 0, 0));
	test.setVelocity(2, glm::vec3(30, 0, 0));
	test.setVelocity(3, glm::vec3(0, 0, 70));


	//test.changeIsMovable(0, false);
	glm::vec3 t = glm::vec3(0, 1, 0);
	glm::vec3 t2 = glm::vec3(-1, 0, 0);
	test.rotateModel(1, 45, t);
	test.rotateModel(1, -45, t2);
	test.rotateModel(0, 8, t2);

	test.startRenderLoop();
}