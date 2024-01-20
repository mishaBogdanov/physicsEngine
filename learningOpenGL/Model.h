#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include<glm/gtx/rotate_vector.hpp>



#include <iostream>
#include "Mesh.h"
#include "Hitbox.h"

struct Contact {
	glm::vec3 position;
	glm::vec3 normal;
};

struct IntersectionModel {
	Model* model1;
	Model* model2;
	//float amountIntersect;
	glm::vec3 solutionVec;
};

struct Impulse {
	glm::vec3 direction;
	glm::vec3 position;
	glm::vec3 centerCollision;
};

struct Friction {
	Model* otherModel;
	glm::vec3 position;
	glm::vec3 value;
};


class Model
{
private:
	std::vector<Mesh> mesh;
	//glm::vec3 pos;
	glm::mat4 translation;
	glm::mat4 inverseTranslation;

	glm::mat3 inertiaTensor;
	glm::mat3 inverseInertiaTensor;



	glm::vec3 facing;
	double currentMouseY;
	double currentMouseX;
	glm::vec3 localY;
	glm::vec3 localZ;
	glm::vec3 localX;

	glm::vec3 velocity;
	glm::vec3 vDueToForce = glm::vec3(0,0,0);

	glm::vec3 angularVelocityDirection;
	double angularVelocity;
	std::vector<ShaderClass> shaders;
	bool movable;
	bool currentlyDriving = false;
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

	//std::vector<float> wallsDistance;


	//std::vector<glm::vec3> corners;
	//std::vector<glm::vec3> originalCorners;
	glm::vec3 cm;
	glm::vec3 originalCm;
	
	//std::vector<glm::vec3> hitboxVectors;

	std::vector<Hitbox> hitboxes;


	std::vector<Impulse> impulses;
	std::vector<Friction> frictions;
	bool frictionShouldApply = true;
	//float maxDistFromCm;
	std::vector<float> distToCenter;
	std::vector<glm::vec3> forces;


	float bounceFactor;
	float mass;
	float frictionFactor;

	float gScale;
	std::string type;

	float bladeRotationAngle = 0;
	float bladeRotationRate = 700;

	std::vector<Model*> debugger;

	void setupModel(float mass);
	void setupCybertruck(float scale);
	bool load(std::string opening, float scale, bool customLocation, glm::vec3 newLocation, bool addHitbox);
	bool loadHitbox(std::string opening, float scale,  glm::vec3 newLocation);
	void createHitbox(float PosX, float PosY, float PosZ, float NegX, float NegY, float NegZ);
	void setupHitbox();
	void loadAppache(float gMass);



	
public:
	Model(std::string filepath);

	Model(std::string filepath, float scale);
	Model(std::string filepath, glm::vec3 location);
	Model(std::string filepath, float scale, glm::vec3 location, int hitbox); // 1 is normal hitbox, 2 is detailed hitbox, 3 is no hitbox
	Model(float length, float gheight, float gz, float gx);

	
	//Model(std::string filepath, glm::vec3 location)


	void Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam);
	void Draw(Camera cam);
	glm::mat4 getTransformation();


	void update(float deltaT);

	void setVelocity(glm::vec3 &givenV);
	void setAngularVelocity(glm::vec3& givenAV);
	void scaleVelocity(float scale);
	void setPosition(glm::vec3 givenPos);
	void addVelocity(glm::vec3& given);

	void getHitboxCorners(std::vector<glm::vec3>& returning);
	void getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min);
	void rotate(float angle, glm::vec3& norm);
	void moveBy(glm::vec3& given);
	float getMass();
	glm::vec3* getCm();
	void addAngularVelocity(glm::vec3& givenVelocity);
	float getMagnitudeVelocity();
	glm::vec3 getVelocity();
	glm::vec3 getFacing();

	std::vector<glm::vec3> getHitboxVectors();

	void getHitboxes(Hitbox * & firstHitbox, int & size);
	int getHitboxesSize();

	void addIntersection(IntersectionModel * given);
	Hitbox* getHitbox(int i);

	bool isMovable();
	void handleCollisions();

	void changeIsMovable(bool value);

	void addImpulse(Impulse imp);
	void dealWithImpulses();
	float getBounceFactor();
	float getFrictionFactor();
	glm::vec3* getAngularVelocity();
	void setMass(float mass);
	void accelerateForward(float acceleration, float deltaT);
	void driveRotate(float degrees, float deltaT);

	bool impulsesEmpty();
	glm::vec3 applyInverseInertiaTensor(glm::vec3 pos, glm::vec3 normal);
	glm::mat4 getInverseTransformation();
	glm::mat4* getRawTransformation();
	void addDebugger(Model* given);
	glm::vec3 velocityAtPoint(glm::vec3& atPoint);
	glm::vec3 getTotalForces();
	void addForce(glm::vec3 force);
	glm::vec3 getForces();
	
	void setCurrentlyDriving(bool given);
	bool getCurrentlyDriving();
	void setCurrentlyFlying(bool given);
	bool getCurrentlyFlying();

	void setWantedRotation(float wanted);
	void setWantedVerticalRotation(float wanted);
	void setWantedRollRotation(float wanted);
	void pitchRotate(float degrees, float deltaT);
	void driveRotateX(float degrees, float deltaT);
	void driveRollX(float degrees, float deltaT);


	void addFrictionForce(Friction gImp);
	void dealWithFrictionForce();
	void upadateFacingdirection(double newMouseX, double newMouseY);
	bool isMoving();
};




