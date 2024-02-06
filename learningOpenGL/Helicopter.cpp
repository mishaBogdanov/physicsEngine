#include "Helicopter.h"
#include "World.h"
#include "MyMath.h"

Helicopter::Helicopter(float mass, Model* givenModdel, World * World) {
	gModel = givenModdel;
	massModdel = mass;
	gWorld = World;
}

void Helicopter::processInput(float dt) {
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
	if (GetKeyState('X') & 0x8000) {
		shoot();
	}
}

void Helicopter::update(float dt) {
	//processInput(dt);
	Impulse imp;
	imp.position = *gModel->getCm();
	imp.direction = propellerForce;
	imp.centerCollision = *gModel->getCm();
	gModel->addImpulse(imp);
}

Model* Helicopter::getModdel() {
	return gModel;
}

void Helicopter::shoot() {
	if (glfwGetTime() - lastShotTime > 0.3) {
		shootingRight = !shootingRight;
		gWorld->addModel("Missile.object", 20,
			*gModel->getZ() * 3.0f * gModel->getScale() +
			*gModel->getY() * (-0.8f) * gModel->getScale() +
			*gModel->getX() * (shootingRight ? 0.7f : -0.7f) * gModel->getScale() +
			*gModel->getCm()
			, true, true);
		gWorld->setVelocity(gWorld->getModelsSize() - 1, *gModel->getZ() * 1000.0f);


		glm::mat4 mat = *gModel->getRawTransformation() * glm::rotate(glm::mat4(1), (float)glm::radians(90.0f), glm::vec3(0, 1, 0));


		gWorld->setTransformation(gWorld->getModelsSize() - 1, mat);

		lastShotTime = glfwGetTime();
	}
	
}