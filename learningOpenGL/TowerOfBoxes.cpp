#include "TowerOfBoxes.h"


TowerOfBoxes::TowerOfBoxes() {
	World test = World(0, 0, 0, 0, true);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	test.addModel("BasicCube.object", 500, glm::vec3(0, -490, 0), true, false);
	test.addModel("BasicCube.object", 30, glm::vec3(50,-9, 50), true, false);


	//test.addVehichle("Apache.object", 20, glm::vec3(-10, 160 + 30, -10));
	//test.addModel("Apache.object", 20, glm::vec3(-10, 160 + 30, -10), true, true);
	test.addModel("TeslaTruck.object", 10, glm::vec3(-10, 200, -10), true, true);

	//test.setMass(2, 40000);
	//test.setVelocity(1, glm::vec3(100, 0, 0));
	//test.addModel("BasicCube.object", 1, glm::vec3(3, 140, 3), false);
	//test.addModel("BasicCube.object", 1, glm::vec3(3, 140, 3), false);
	//test.addModel("BasicCube.object", 1, glm::vec3(3, 140, 3), false);
	//test.addModel("BasicCube.object", 1, glm::vec3(3, 140, 3), false);
	//test.addModel("BasicCube.object", 1, glm::vec3(3, 140, 3), false);
	//test.addModel("BasicCube.object", 1, glm::vec3(3, 140, 3), false);


	//test.setVelocity(1, glm::vec3(0, -10, 0));

	test.addModel("BasicCube.object", 27, glm::vec3(100, 280 + 50, 100), true, true);
	test.addModel("BasicCube.object", 33, glm::vec3(100, 320 + 50, 100), true, true);
	test.addModel("BasicCube.object", 55, glm::vec3(100, 340 + 50, 310), true, true);
	test.addModel("BasicCube.object", 13, glm::vec3(100, 360 + 50, 100), true, true);
	test.addModel("BasicCube.object", 12, glm::vec3(100, 400 + 50, 100), true, true);
	test.addModel("BasicCube.object", 28, glm::vec3(140, 440 + 50, 100), true, true);
	//test.addModel("BasicCube.object", 5, glm::vec3(3, 460 + 50, 3), true, true);
	//test.addModel("BasicCube.object", 9, glm::vec3(3, 460 + 50, 3), true, true);
	//test.addModel("BasicCube.object", 14, glm::vec3(3, 280 + 50, 3), true, true);
	//test.addModel("BasicCube.object", 19, glm::vec3(3, 320 + 50, 3), true, true);
	//test.addModel("BasicCube.object", 14, glm::vec3(3, 280 + 50, 3), true, true);
	//test.addModel("BasicCube.object", 19, glm::vec3(3, 320 + 50, 3), true, true);
	//test.addModel("BasicCube.object", 5, glm::vec3(2, 360, 3), true, true);
	//test.addModel("BasicCube.object", 6, glm::vec3(3, 400, 1), true, true);
	//test.addModel("BasicCube.object", 10, glm::vec3(3, 440, 3), true, true);
	//test.addModel("TeslaTruck.object", 30, glm::vec3(2, 480, 0), true, true);
	//test.addModel("BasicCube.object", 5, glm::vec3(4, 220, 0), true, true);
	//test.addModel("Domino.object", 5, glm::vec3(4, 20, 0), true, true);
	





	//test.changeIsMovable(0, false);
	//test.changeIsMovable(2, false);
	//test.changeIsMovable(3, false);
	//test.changeIsMovable(4, false);

	//test.setVelocity(1, glm::vec3(0, -90, 0));

	//test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
	//test.setVelocity(0, glm::vec3(0, 0, 40));
	//test.setVelocity(1, glm::vec3(-30, 0, 0));
	//test.setVelocity(2, glm::vec3(30, 0, 0));
	//test.setVelocity(3, glm::vec3(0, 0, 30));


	//test.changeIsMovable(0, false);
	glm::vec3 t = glm::vec3(0, 0, 1);
	glm::vec3 t2 = glm::vec3(1, 0, 0);
	test.rotateModel(3, 20, t);
	test.rotateModel(1, 20, t);

	//test.rotateModel(1, 35.2643896828, t2);
	//test.rotateModel(1, 20, t2);

	//test.rotateModel(1, -45, t2);
	//test.rotateModel(0, 8, t2);

	test.startRenderLoop();
}