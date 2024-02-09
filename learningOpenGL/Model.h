#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include<glm/gtx/rotate_vector.hpp>

#include "Mesh.h"
#include "MyMath.h"
#include "vectorPrintingClass.h"




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
protected:

	//list of meshes that need to be drawn
	std::vector<Mesh> mesh;

	// the main translation, and inverse translation matricies
	glm::mat4 translation;
	glm::mat4 inverseTranslation;

	// the inertia and inverse inertia tensors. used for collisions handling.
	glm::mat3 inertiaTensor;
	glm::mat3 inverseInertiaTensor;


	// used for controls and friction. should be changed to be a local variable in the car and heli classes
	glm::vec3 facing;
	glm::vec3 localY;
	glm::vec3 localZ;
	glm::vec3 localX;

	// velocity, and v after force. different because of collision handling. 
	// sometimes you don't want to add force if an object is resting.
	glm::vec3 velocity;
	glm::vec3 vDueToForce = glm::vec3(0,0,0);

	// angular velocity. is updated every frame by the right hand rule, and the magnitude of the vector.
	glm::vec3 angularVelocityDirection;




	// is true if an object can be moved, is false if it can't
	bool movable;

	// !!! to remove and put into another class.
	bool currentlyDriving = false;
	bool currentlyFlying = false;

	// !!! to remove and put into it's own class
	float steeringRate = 20.0f;
	float wantedSteeringPos = 0;
	float currentSteeringPosition = 0;


	float verticalSteeringRate = 20.0f;
	float wantedVerticalSteeringPos = 0;
	float currentVerticalSteeringPosition = 0;

	float rollSteeringRate = 20.0f;
	float wantedRollSteeringPos = 0;
	float currentRollSteeringPosition = 0;


	// used for physics.
	glm::vec3 cm;
	glm::vec3 originalCm;
	
	// list of asociated hitboxes.
	std::vector<Hitbox> hitboxes;

	// list of impulses that add up and get added
	std::vector<Impulse> impulses;

	// list of frictions
	std::vector<Friction> frictions;
	bool frictionShouldApply = true;
	
	// is used to calculate the inertia tensors
	std::vector<float> distToCenter;

	// the list of forces acting on the object
	std::vector<glm::vec3> forces;

	// a couple physics properties
	float bounceFactor;
	float mass;
	float frictionFactor;

	// the size of the object
	float gScale;

	// !!!
	std::string type;

	// !!!
	float bladeRotationAngle = 0;
	float bladeRotationRate = 700;

	std::vector<Model*> debugger;

	void setupModel(float mass);

	// !!!
	void setupCybertruck(float scale);
	bool load(std::string opening, float scale, bool customLocation, glm::vec3 newLocation, bool addHitbox);
	bool loadHitbox(std::string opening, float scale,  glm::vec3 newLocation);
	void createHitbox(float PosX, float PosY, float PosZ, float NegX, float NegY, float NegZ);
	void setupHitbox();
	virtual void loadAppache(float gMass);
	virtual void updatePhysics(float deltaT);
	virtual void updateMeshes(glm::mat4& given);
	void setupMissile();


	
public:
	Model(std::string filepath);

	Model(std::string filepath, float scale);
	Model(std::string filepath, glm::vec3 location);
	Model(std::string filepath, float scale, glm::vec3 location, int hitbox); // 1 is normal hitbox, 2 is detailed hitbox, 3 is no hitbox
	Model(float length, float gheight, float gz, float gx);



	virtual void Draw(Camera &cam, ShaderClass & shaders);
	glm::mat4 getTransformation();
	void setTransformation(glm::mat4 gTransfomation);


	virtual void update(float deltaT);

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
	virtual void dealWithImpulses();
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
	
	// !!!
	virtual void setCurrentlyDriving(bool given);
	virtual bool getCurrentlyDriving();
	virtual void setCurrentlyFlying(bool given);
	virtual bool getCurrentlyFlying();

	virtual void setWantedRotation(float wanted);
	virtual void setWantedVerticalRotation(float wanted);
	virtual void setWantedRollRotation(float wanted);
	virtual void pitchRotate(float degrees, float deltaT);
	virtual void driveRotateX(float degrees, float deltaT);
	virtual void driveRollX(float degrees, float deltaT);


	void addFrictionForce(Friction gImp);
	virtual void dealWithFrictionForce();
	void upadateFacingdirection(double newMouseX, double newMouseY);
	bool isMoving();
	glm::vec3* getZ();
	glm::vec3* getY();
	glm::vec3* getX();
	float getScale();
	std::string getType();
	void setScale(float gScale);
};




