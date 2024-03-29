#pragma once
#include "Drivable.h"

class World;

class UpdatedHelicopter : public Drivable
{
private:
	glm::vec3 facing;
	glm::vec3 localY;
	glm::vec3 localZ;
	glm::vec3 localX;
	//std::vector<ShaderClass> shaders;
	bool currentlyFlying = false;

	float steeringRate = 20.0f;
	float wantedSteeringPos = 0;
	float currentSteeringPosition = 0;


	float verticalSteeringRate = 20.0f;
	float wantedVerticalSteeringPos = 0;
	float currentVerticalSteeringPosition = 0;

	float rollSteeringRate = 20.0f;
	float wantedRollSteeringPos = 0;
	float currentRollSteeringPosition = 0;

	float bladeRotationAngle = 0;
	float bladeRotationRate = 700;

	glm::vec3 x = glm::vec3(1, 0, 0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);
	World* gWorld;
	bool shootingRight;
	float lastShotTime = 0;
	glm::vec3 propellerForce = glm::vec3(0, 0, 0);


public:

	UpdatedHelicopter(float scale, glm::vec3 physLocation, int hitbox);
	void handleInputs(float dt) override;
	void update(float deltaT) override;
	void dealWithImpulses() override;
	void cameraControls(Camera & cam, GLFWwindow* window) override;


	void loadAppache(float gMass) override;
	void setCurrentlyFlying(bool given) override;
	bool getCurrentlyFlying() override;
	void setWantedRotation(float wanted) override;
	void setWantedVerticalRotation(float wanted) override;
	void setWantedRollRotation(float wanted) override;
	void pitchRotate(float degrees, float deltaT)override;
	void driveRotateX(float degrees, float deltaT) override;
	void driveRollX(float degrees, float deltaT) override;


};

