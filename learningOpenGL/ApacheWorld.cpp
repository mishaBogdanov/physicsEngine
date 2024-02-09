#include "ApacheWorld.h"


ApacheWorld::ApacheWorld() {
	World test = World(0, 0, 0, 0, true);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	test.setupExplosions();
	test.setupShaders();
	test.addModel("BasicCube.object", 1000, glm::vec3(0, -980, 0), true, false);
	//test.addModel("TreeArea.object", 10, glm::vec3(0, 60, 0), false, false);


	test.addVehichle("Apache.object", 20, glm::vec3(-500, 160 + 30, -500));
	test.addModel("WallObject.object", 20, glm::vec3(190, 120, 0), true, true);
	test.addModel("WallObject.object",20, glm::vec3(0, 120, 190), true, true);
	test.addModel("WallObject.object", 20, glm::vec3(-190, 120, 0), true, true);
	test.addModel("WallObject.object", 20, glm::vec3(0, 120, -190), true, true);

	//t1
	test.addModel("BasicCube.object", 20, glm::vec3(-130, 60, -130), true, true);
	test.addModel("BasicCube.object", 16, glm::vec3(-130, 100, -130), true, true);
	test.addModel("BasicCube.object", 14, glm::vec3(-130, 140, -130), true, true);
	test.addModel("BasicCube.object", 12, glm::vec3(-130, 180, -130), true, true);

	//t2
	test.addModel("BasicCube.object", 20, glm::vec3(130, 60, 130), true, true);
	test.addModel("BasicCube.object", 16, glm::vec3(130, 100, 130), true, true);
	test.addModel("BasicCube.object", 14, glm::vec3(130, 140, 130), true, true);
	test.addModel("BasicCube.object", 12, glm::vec3(130, 180, 130), true, true);

	//t3
	test.addModel("BasicCube.object", 20, glm::vec3(-130, 60, 130), true, true);
	test.addModel("BasicCube.object", 16, glm::vec3(-130, 100, 130), true, true);
	test.addModel("BasicCube.object", 14, glm::vec3(-130, 140, 130), true, true);
	test.addModel("BasicCube.object", 12, glm::vec3(-130, 180, 130), true, true);

	//t4
	test.addModel("BasicCube.object", 20, glm::vec3(130, 60, -130), true, true);
	test.addModel("BasicCube.object", 16, glm::vec3(130, 100, -130), true, true);
	test.addModel("BasicCube.object", 14, glm::vec3(130, 140, -130), true, true);
	test.addModel("BasicCube.object", 12, glm::vec3(130, 180, -130), true, true);


	test.addModel("Tank.object", 10, glm::vec3(40, 180, -50), true, true);
	test.addModel("Tank.object", 10, glm::vec3(110, 180, -30), true, true);
	test.addModel("Tank.object", 10, glm::vec3(-50, 180, -50), true, true);
	test.addModel("TeslaTruck.object", 10, glm::vec3(-110, 180, -50), true, true);
	test.addModel("House.object", 10, glm::vec3(0, 180, 90), true, true);
	test.testAddHelicopter();




	glm::vec3 up = glm::vec3(0, 1, 0);
	test.rotateModel(3, 90, up);
	test.rotateModel(5, 90, up);
	test.rotateModel(26, 90, up);

	//test.addModel("WallObject.object", 10, glm::vec3(0, 150, 20), true, true);
	//test.addModel("")

	//test.addModel("BasicCube.object", 33, glm::vec3(100, 345 + 50, 100), true, true);
	//test.addModel("BasicCube.object", 55, glm::vec3(100, 340 + 50, 310), true, true);
	//test.addModel("BasicCube.object", 13, glm::vec3(100, 415 + 50, 100), true, true);
	//test.addModel("BasicCube.object", 10, glm::vec3(100, 440 + 50, 100), true, true);
	//test.addModel("BasicCube.object", 28, glm::vec3(190, 440 + 50, 100), true, true);


	//glm::vec3 t = glm::vec3(0, 0, 1);
	//glm::vec3 t2 = glm::vec3(1, 0, 0);
	//test.rotateModel(3, 20, t);
	//test.rotateModel(1, 20, t);


	test.startRenderLoop();
}