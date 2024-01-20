#include "Apache.h"
#include "World.h"
#include "MyMath.h"

Apache::Apache(float mass, Model* givenModdel, World * World) {
	gModel = givenModdel;
	massModdel = mass;
	gWorld = World;
}

void Apache::processInput(float dt) {
	glm::vec3 fDirection= *gModel->getRawTransformation() * glm::vec4(0, 1, 0, 0);
	if (GetKeyState(VK_SPACE) & 0x8000) {
		propellerForce += fDirection * massModdel * 0.2f * dt;
	}
	else if (GetKeyState('C') & 0x8000) {
		propellerForce -= fDirection * massModdel * 0.5f * dt;
	}
	else {
		propellerForce = fDirection * MyMath::getVectorMagnitude( * gWorld -> getGravity()) * gModel->getMass() * dt;
	}


	if (GetKeyState(VK_UP) & 0x8000) {
		gModel->setWantedVerticalRotation(-4.0);
	}else if (GetKeyState(VK_DOWN) & 0x8000) {
		gModel->setWantedVerticalRotation(4.0);
	}
	else {
		gModel->setWantedVerticalRotation(0);

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

	if (GetKeyState('D') & 0x8000) {
		gModel->setWantedRollRotation(-4.0);
	}
	else if (GetKeyState('A') & 0x8000) {
		gModel->setWantedRollRotation(4.0);
	}
	else {
		gModel->setWantedRollRotation(0.0);
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