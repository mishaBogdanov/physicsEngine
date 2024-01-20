#include "rawTesla.h"


RawTesla::RawTesla() {
	World test = World(0, 0, 0, 0, false);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	test.addModel("TeslaTruck.object", 30, glm::vec3(0, 9 + 30, 0), true, true);
	test.addModel("BasicCube.object", 0.1, glm::vec3(100, 100, 100), true, true);


	test.startRenderLoop();
}