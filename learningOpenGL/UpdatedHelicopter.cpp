#include "UpdatedHelicopter.h"
#include "World.h"

UpdatedHelicopter::UpdatedHelicopter(float scale, glm::vec3 physLocation, int hitbox) : Drivable ("Apache.object", scale, physLocation, hitbox) {
	

	load("Apache.object", scale, true, physLocation, true);
	loadAppache(scale * scale * scale * 2);
	gScale = scale;
	type = "A";
}


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

void UpdatedHelicopter::update(float deltaT) {
	Model::updatePhysics(deltaT);
	Impulse imp;
	imp.position = *getCm();
	imp.direction = propellerForce;
	imp.centerCollision = *getCm();
	addImpulse(imp);
	localY = translation * glm::vec4(0, 1, 0, 0);
	localX = translation * glm::vec4(1, 0, 0, 0);
	localZ = translation * glm::vec4(0, 0, 1, 0);
	if (wantedSteeringPos != currentSteeringPosition) {
		if (currentSteeringPosition > wantedSteeringPos) {
			currentSteeringPosition = max(currentSteeringPosition - deltaT * steeringRate, wantedSteeringPos);
		}
		else {
			currentSteeringPosition = min(currentSteeringPosition + deltaT * steeringRate, wantedSteeringPos);
		}
	}
	if (wantedVerticalSteeringPos != currentVerticalSteeringPosition) {
		if (currentVerticalSteeringPosition > wantedVerticalSteeringPos) {
			currentVerticalSteeringPosition = max(currentVerticalSteeringPosition - deltaT * verticalSteeringRate, wantedVerticalSteeringPos);
		}
		else {
			currentVerticalSteeringPosition = min(currentVerticalSteeringPosition + deltaT * verticalSteeringRate, verticalSteeringRate);
		}
	}
	if (wantedRollSteeringPos != currentRollSteeringPosition) {
		if (currentRollSteeringPosition > wantedRollSteeringPos) {
			currentRollSteeringPosition = max(currentRollSteeringPosition - deltaT * rollSteeringRate, wantedRollSteeringPos);
		}
		else {
			currentRollSteeringPosition = min(currentRollSteeringPosition + deltaT * rollSteeringRate, rollSteeringRate);
		}
	}
	if (currentlyFlying) {
		velocity = velocity * (float)pow(2, glm::length(velocity) * deltaT * (-0.01f));
	}
	if (currentVerticalSteeringPosition != 0) {
		pitchRotate(currentVerticalSteeringPosition, deltaT);
	}
	if (currentSteeringPosition != 0) {
		driveRotate(currentSteeringPosition, deltaT);
	}
	if (currentRollSteeringPosition != 0) {
		driveRollX(currentRollSteeringPosition, deltaT);
	}
	if (MyMath::getVectorMagnitudeSquared(angularVelocityDirection) < 0.0000000001) {
		angularVelocityDirection = glm::vec3(0);
	}
	else {
		angularVelocityDirection = angularVelocityDirection * (float)pow(2.6, -deltaT);
	}


	glm::mat4 position = getTransformation();
	glm::mat4 bladeRotation = getTransformation()
		* glm::translate(glm::mat4(1), glm::vec3(0, 0, +0.65) * gScale) *
		glm::rotate(glm::mat4(1), glm::radians(bladeRotationAngle), glm::vec3(0, 1, 0)) * glm::translate(glm::mat4(1), glm::vec3(0, 0, -0.65) * gScale);
	bladeRotationAngle += bladeRotationRate * deltaT;
	mesh[0].updateTransLocation(bladeRotation);
	mesh[1].updateTransLocation(position);
}

void UpdatedHelicopter::dealWithImpulses() {
	glm::vec3 vUpdate = glm::vec3(0);
	glm::vec3 aVUpdate = glm::vec3(0);
	bool initialize = false;
	frictionShouldApply = false;
	for (int i = 0; i < impulses.size(); i++) {
		if (glm::length(impulses[i].direction) != 0) {
			initialize = true;
			glm::vec3 cmToImpulse = impulses[i].position - cm;

			glm::vec3 cmToCmImpulseTranslated = inverseTranslation * glm::vec4(cmToImpulse, 1.0f);
			glm::vec3 translatedImpulse = inverseTranslation * glm::vec4(impulses[i].direction, 1.0f);
			glm::vec3 changeInAngular = inverseInertiaTensor * glm::cross(cmToCmImpulseTranslated, translatedImpulse);
			glm::vec3 changeInAngularInOriginal = translation * glm::vec4(changeInAngular, 1.0f);

			vUpdate += impulses[i].direction / mass;
			aVUpdate += changeInAngularInOriginal * 1.0f;
		}
	}
	if (initialize) {
		if (MyMath::getVectorMagnitudeSquared(vUpdate) > 0.75 || MyMath::getVectorMagnitudeSquared(velocity) > 0.9 ||
			MyMath::getVectorMagnitudeSquared(aVUpdate) > 0.0001 || MyMath::getVectorMagnitudeSquared(angularVelocityDirection) > 0.01
			|| currentlyFlying) {
			velocity += vUpdate;
			frictionShouldApply = true;
			angularVelocityDirection += aVUpdate;

		}
		else {
			velocity = glm::vec3(0);
			angularVelocityDirection = glm::vec3(0);
		}
	}
	impulses.clear();
}

void UpdatedHelicopter::setCurrentlyFlying(bool given) {
	currentlyFlying = true;
}

bool UpdatedHelicopter::getCurrentlyFlying() {
	return currentlyFlying;
}




void UpdatedHelicopter::setWantedRotation(float wanted) {
	wantedSteeringPos = wanted;
}


void UpdatedHelicopter::setWantedVerticalRotation(float wanted) {
	wantedVerticalSteeringPos = wanted;
}

void UpdatedHelicopter::setWantedRollRotation(float wanted) {
	wantedRollSteeringPos = wanted;
}


void UpdatedHelicopter::pitchRotate(float degrees, float deltaT) {
	if (currentlyFlying) {
		rotate(-degrees * deltaT * 100 / 7, localX);
	}
}

void UpdatedHelicopter::driveRotateX(float degrees, float deltaT) {
	rotate(-degrees * glm::length(velocity) * deltaT / 7, localX);
}

void UpdatedHelicopter::driveRollX(float degrees, float deltaT) {
	rotate(-degrees * glm::length(velocity) * deltaT / 7, localZ);
}

void UpdatedHelicopter::cameraControls(Camera & cam, GLFWwindow * window) {
	cam.setVehichleCamera(*getRawTransformation(), *getCm(), window);
}


void UpdatedHelicopter::loadAppache(float gmass) {
	localY = glm::vec3(0, 1, 0);
	localX = glm::vec3(1, 0, 0);
	localZ = glm::vec3(0, 0, 1);
	mass = gmass;
	translation = glm::mat4(1);
	inverseTranslation = glm::mat4(1);
	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(0, 0, 0);
	facing = glm::vec3(0, 0, 1);


	movable = true;


	bounceFactor = 0.3f;
	frictionFactor = 0.7f;
	glm::vec3 negCm = originalCm * (-1.0f);
	glm::vec3 originalCmSet = originalCm;
	moveBy(negCm);

	moveBy(originalCmSet);

	for (int i = 0; i < distToCenter.size(); i += 2) {
		//std::cout << distToCenter[i] << " " << distToCenter[i + 1] << "\n";
		float average = distToCenter[i] - distToCenter[i + 1];
		distToCenter[i] = average / 2;
		distToCenter[i + 1] = average / (-2);

	}


	inertiaTensor = glm::mat3(1);

	inertiaTensor[0][0] = mass / 12 * (pow(distToCenter[2] - distToCenter[3], 2) + (pow(distToCenter[4] - distToCenter[5], 2)));
	//std::cout << "got here \n";

	inertiaTensor[1][1] = mass / 12 * (pow(distToCenter[0] - distToCenter[1], 2) + (pow(distToCenter[4] - distToCenter[5], 2)));
	inertiaTensor[2][2] = mass / 12 * (pow(distToCenter[0] - distToCenter[1], 2) + (pow(distToCenter[2] - distToCenter[3], 2)));
	inverseInertiaTensor = glm::inverse(inertiaTensor);
}
