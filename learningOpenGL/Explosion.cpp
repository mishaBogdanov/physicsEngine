#include "Explosion.h"
#include "Camera.h"



void Explosion::update(float dt) {
	if (active) {
		//std::cout << scale << "\n";
		scale += scaleRate * dt;
		gModel -> setScale(scale);
		gModel->update(1);
		std::cout << (*gModel->getRawTransformation())[0][0] << "\n";

		if (scale >= maxScale) {
			active = false;
			scale = 0;
		}
	}
}

void Explosion::draw() {
	if (active) {
		gModel->Draw(*gCam);
	}
}

void Explosion::enable(glm::vec3 givenPos) {
	active = true;
	gModel->setPosition(givenPos);
	gModel->update(1);
}

Explosion::Explosion(Model * model, Camera * cam) {
	gModel = model;
	gCam = cam;
}

bool Explosion::getEnabled() {
	return active;
}

