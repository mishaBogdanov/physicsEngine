#include "RoomWorld.h"

RoomWorld::RoomWorld() {
	World test = World(0, 0, 0, 0, true);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	//test.addModel("BasicCube.object", 15, glm::vec3(3, 140, 3), true, true);

	test.addModel("simpleBox.object", 4, glm::vec3(0 + 40, 30, 0 + 40), false, false);
	test.addHitbox("simpleBox.object", 4, glm::vec3(0 + 40, 30, 0 +40));


	test.addModel("BasicCube.object", 10, glm::vec3(3 +40, 180, 3 + 40), true, true);

	//test.addModel("BasicCube.object", 100, glm::vec3(0, -90, 0), true);
	test.addModel("BasicCube.object", 5, glm::vec3(3 + 40, 290, 3+ 40), true, true);
	//test.addModel("BasicCube.object", 6, glm::vec3(3, 170, 3), true);

	//test.addModel("BasicCube.object", 2, glm::vec3(3, 200, 3), true);

	//test.addModel("BasicCube.object", 15, glm::vec3(2, 190, 0), true);
	//test.addModel("BasicCube.object", 5, glm::vec3(4, 220, 0), true);

	test.changeIsMovable(1, false);


	//test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
	//test.setVelocity(0, glm::vec3(0, 0, 40));
	//test.setVelocity(1, glm::vec3(-30, 0, 0));
	//test.setVelocity(2, glm::vec3(30, 0, 0));
	//test.setVelocity(3, glm::vec3(0, 0, 30));


	//test.changeIsMovable(0, false);
	glm::vec3 t = glm::vec3(1, 1, 0);
	//glm::vec3 t2 = glm::vec3(-1, 0, 0);
	//test.rotateModel(2, 20, t);
	//test.rotateModel(1, -45, t2);
	//test.rotateModel(0, 8, t2);

	test.startRenderLoop();
}
