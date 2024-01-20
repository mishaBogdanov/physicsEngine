#include "Simple2BoxCollision.h"


Simple2BoxCollision::Simple2BoxCollision() {
	World test = World(0, 0, 0, 0, false);
	test.setupGLFW();
	test.addFloor(10, 100, -60, -60, 0);
	test.addModel("BasicCube.object", 10, glm::vec3(0, 0, 0), true, true);
	test.addModel("BasicCube.object", 7, glm::vec3(0, 0, 32), true, true);
	//test.addModel("BasicCube.object", 15, glm::vec3(2, 190, 0), true);
	//test.addModel("BasicCube.object", 5, glm::vec3(4, 220, 0), true);

	//test.changeIsMovable(0, false);
	//test.changeIsMovable(0, false);

	//test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
	test.setVelocity(1, glm::vec3(0, 0, -10));
	test.setAngularVelocity(1, glm::vec3(40, 0, 0));

	//test.setVelocity(1, glm::vec3(-30, 0, 0));
	//test.setVelocity(2, glm::vec3(30, 0, 0));
	//test.setVelocity(3, glm::vec3(0, 0, 30));


	//test.changeIsMovable(0, false);
	glm::vec3 t = glm::vec3(1, 1, 0);
	glm::vec3 t2 = glm::vec3(-1, 0, 0);
	//test.rotateModel(1, 20, t);
	//test.rotateModel(0, 0, t2);
	//test.rotateModel(0, 8, t2);

	test.startRenderLoop();
}