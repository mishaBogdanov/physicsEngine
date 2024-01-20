#include "DominoesWorld.h"

DominoesWorld::DominoesWorld() {
	World test = World(0, 0, 0, 0, true);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	test.addModel("BasicCube.object", 500, glm::vec3(0, -490, 0), true, false);
	test.addModel("BasicCube.object", 30, glm::vec3(50, -9, 50), true, false);

	test.addModel("TeslaTruck.object", 10, glm::vec3(-10, 160 + 30, -10), true, true);
	test.setMass(2, 10000);

	test.addModel("BasicCube.object", 25, glm::vec3(-100, 10, 100), true, true);
	test.addModel("BasicCube.object", 50, glm::vec3(100, 10, 100), true, false);
	for (int i = 0; i < 40; i++) {
		test.addModel("Domino.object", 5, glm::vec3(i * 5 + 40, 20, 0), true, true);
	}
	glm::vec3 t = glm::vec3(0, 0, 1);
	glm::vec3 t2 = glm::vec3(1, 0, 0);
	test.rotateModel(3, 20, t);
	test.rotateModel(1, 20, t);

	test.startRenderLoop();
}
