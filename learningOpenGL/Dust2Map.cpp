#include "Dust2Map.h"


Dust2Map::Dust2Map() {
	World test = World(0, 0, 0, 0, true);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);

	test.addModel("Dust2Raw.object", 4.0f, glm::vec3(0, 140, 0), false, false);
	test.addHitbox("Dust2Hitbox.object", 4.0f, glm::vec3(0, 140, 0));

	test.addModel("TeslaTruck.object", 16, glm::vec3(120 * 2, 160, 400 * 2), true, true);
	//test.addModel("SimpleCube.object", 3, glm::vec3(190, 160, 400), true, true);
	//test.addModel("SimpleCube.object", 2.8, glm::vec3(260, 160, 400), true, true);
	//test.addModel("SimpleCube.object", 2.6, glm::vec3(160, 160, 400), true, true);
	//test.addModel("SimpleCube.object", 2, glm::vec3(160, 160, 400), true, true);

	
	test.addModel("BasicCube.object", 9 * 2, glm::vec3(120 * 2, 160 * 2, 470 * 2), true, true);

	test.addModel("BasicCube.object", 8 * 2, glm::vec3(120 * 2, 180 * 2, 470 * 2), true, true);

	test.addModel("BasicCube.object", 7 * 2, glm::vec3(120 * 2, 200 * 2, 470 * 2), true, true);
	test.addModel("BasicCube.object", 6 * 2, glm::vec3(120 * 2, 220 * 2, 470 * 2), true, true);


	//test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
	//test.setVelocity(0, glm::vec3(0, 0, 40));
	//test.setVelocity(1, glm::vec3(-300, 0, 0));
	//test.setVelocity(2, glm::vec3(30, 0, 0));
	//test.setVelocity(3, glm::vec3(0, 0, 70));


	//test.changeIsMovable(0, false);
	//glm::vec3 t = glm::vec3(0, 1, 0);
	//glm::vec3 t2 = glm::vec3(-1, 0, 0);
	//test.rotateModel(1, 45, t);
	//test.rotateModel(1, -45, t2);
	//test.rotateModel(0, 8, t2);

	test.startRenderLoop();
}