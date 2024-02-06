#include "UpdatedHelicopter.h"
#include "World.h"

void UpdatedHelicopter::handleInputs(float dt) {
	glm::vec3 fDirection = *getRawTransformation() * glm::vec4(0, 1, 0, 0);
	if (GetKeyState(VK_SPACE) & 0x8000) {
		propellerForce += fDirection * mass * 0.2f * dt;
	}
	else if (GetKeyState('C') & 0x8000) {
		propellerForce -= fDirection * mass * 0.5f * dt;
	}
	else {
		propellerForce = fDirection * MyMath::getVectorMagnitude(*gWorld->getGravity()) * mass * dt;
	}


	if (GetKeyState(VK_UP) & 0x8000) {
		setWantedVerticalRotation(-4.0);
	}
	else if (GetKeyState(VK_DOWN) & 0x8000) {
		setWantedVerticalRotation(4.0);
	}
	else {
		setWantedVerticalRotation(0);

	}

	if (GetKeyState(VK_RIGHT) & 0x8000) {
		setWantedRotation(4.0);
	}
	else if (GetKeyState(VK_LEFT) & 0x8000) {
		setWantedRotation(-4.0);
	}
	else {
		setWantedRotation(0.0);
	}

	if (GetKeyState('D') & 0x8000) {
		setWantedRollRotation(-4.0);
	}
	else if (GetKeyState('A') & 0x8000) {
		setWantedRollRotation(4.0);
	}
	else {
		setWantedRollRotation(0.0);
	}
	if (GetKeyState('X') & 0x8000) {
		//shoot();
	}
}
