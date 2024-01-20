#include "Apache.h"


Apache::Apache(float mass, Model* givenModdel) {
	gModel = givenModdel;
	massModdel = mass;
}

void Apache::processInput(float dt) {
	if (GetKeyState(VK_SPACE) & 0x8000) {
		propellerForce += x * massModdel * 0.2f * dt;
	}


	if (GetKeyState(VK_UP) & 0x8000) {
		gModel->setWantedVerticalRotation(4.0);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000) {
		gModel->setWantedRotation(4.0);
	}
	else if (GetKeyState(VK_LEFT) & 0x8000) {
		gModel->setWantedRotation(-4.0);
	}
	else {
		gModel->setWantedRotation(0.0);
	}

	if (GetKeyState(VK_DOWN) & 0x8000) {
		gModel->setWantedVerticalRotation(-4.0);

	}

	//if (GetKeyState('R') & 0x8000) {
	//	glm::vec3 v = glm::vec3(0);
	//	drivable->setVelocity(v);
	//}
}

void Apache::update(float dt) {
	//processInput(dt);
	Impulse imp;
	imp.position = *gModel->getCm();
	imp.direction = propellerForce;
	imp.centerCollision = *gModel->getCm();
	gModel->addImpulse(imp);
}

Model* Apache::getModdel() {
	return gModel;
}