#include "Model.h"
#pragma warning(disable:4996)
#include "Mesh.h"
#include <iostream>
#include <cstdio>
#include "MyMath.h"
#include "vectorPrintingClass.h"




Model::Model(std::string location) {
	load(location, 1, false, glm::vec3(0, 0, 0), 1);
	setupModel(100);
}

void Model::setupModel(float gmass) {
	localY = glm::vec3(0, 1, 0);
	localX = glm::vec3(1, 0, 0);
	localZ = glm::vec3(0, 0, 1);
	mass = gmass;
	translation = glm::mat4(1);
	inverseTranslation = glm::mat4(1);
	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(0, 0, 0);
	angularVelocity = 0;
	facing = glm::vec3(0, 0, 1);
	ShaderClass shaderProgram("default.vert", "default.geom", "default.frag");
	ShaderClass shaderProgram2("default.vert", "outline.geom", "outline.frag");
	shaders.push_back(shaderProgram);
	shaders.push_back(shaderProgram2);
	for (int k = 0; k < shaders.size(); k++) {
		shaders[k].Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaders[k].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
	}


	movable = true;
	

	bounceFactor = 0.3f;
	frictionFactor = 0.7f;
	glm::vec3 negCm = originalCm * (-1.0f);
	glm::vec3 originalCmSet = originalCm;
	moveBy(negCm);

	moveBy(originalCmSet);

	for (int i = 0; i < distToCenter.size(); i+=2) {
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


void Model::setupHitbox() {
	mass = 100;
	translation = glm::mat4(1);
	inverseTranslation = glm::mat4(1);

	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(0, 0, 0);
	angularVelocity = 0;
	facing = glm::vec3(0, 0, 1);

	movable = false;


	bounceFactor = 0.7f;
	frictionFactor = 0.7f;

}




Model::Model(std::string location, float scale) {
	load(location, scale, false, glm::vec3(0, 0, 0), 1);
	setupModel(100);
}

Model::Model(std::string location, glm::vec3 physLocation) {
	load(location, 1, true, physLocation, 1);
	setupModel(100);
}

//Model::Model(float length, float gheight, float gz, float gx) {
//	angularVelocity = 0;
//	angularVelocityDirection = glm::vec3(0);
//	cm = glm::vec3(gx, gheight, gz);
//	facing = glm::vec3(0, 1, 0);
//	mass = 100;
//	originalCm = cm;
//	translation = glm::mat4(1);
//	up = glm::vec3(0, 1, 0);
//	velocity = glm::vec3(0, 0, 0);
//	hitboxVectors.push_back(glm::vec3(0, 1, 0));
//	corners.push_back(glm::vec3(gx - length / 2, gheight, gz - length / 2));
//	corners.push_back(glm::vec3(gx - length / 2, gheight, gz + length / 2));
//	corners.push_back(glm::vec3(gx + length / 2, gheight, gz - length / 2));
//	corners.push_back(glm::vec3(gx + length / 2, gheight, gz + length / 2));
//
//
//	movable = false;
//}


Model::Model(std::string location, float scale, glm::vec3 physLocation, int hitbox) {
	if (location == "Apache.object") {
		load(location, scale, true, physLocation, true);
		loadAppache(scale * scale * scale * 2);
		gScale = scale;
		type = "A";
	}
	else if (hitbox == 2) {
		loadHitbox(location, scale, physLocation);
		setupHitbox();
	}
	else if (hitbox == 1) {

		load(location, scale, true, physLocation, true);

		setupModel(scale * scale * scale * 2);

	}
	else if (hitbox == 3) {
		load(location, scale, true, physLocation, false);
		setupModel(scale * scale * scale * 2);
	}

}






void Model::Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		mesh[i].Draw(shader, cam);
		mesh[i].Draw(shader2, cam);
	}
}

glm::mat4 Model::getTransformation() {
	return glm::translate(glm::mat4(1), cm) * translation * glm::translate(glm::mat4(1), -originalCm);
}

//glm::mat4 Model::getInverseTransformation() {
//	return glm::translate(glm::mat4(1), -cm) * 
//}

void Model::Draw(Camera cam) {
	if (type == "") {
		for (int i = 0; i < mesh.size(); i++) {
			for (int k = 0; k < shaders.size(); k++) {
				mesh[i].Draw(shaders[k], cam);
			}

		}
	}
	else if (type == "A") {
		mesh[1].Draw(shaders[0], cam);
		mesh[1].Draw(shaders[1], cam);
		mesh[0].Draw(shaders[2], cam);
		mesh[0].Draw(shaders[3], cam);

	}
	
}

void Model::update(float deltaT) {

	cm = cm + velocity * deltaT;
	//vpc::printVec(cm);
	
	if (angularVelocityDirection.x != 0 || angularVelocityDirection.y != 0 || angularVelocityDirection.z != 0) {
		glm::mat4 t = glm::rotate(glm::mat4(1), MyMath::getVectorMagnitude(angularVelocityDirection) * deltaT, angularVelocityDirection);
		translation = t * translation;
		inverseTranslation = glm::inverse(translation);

	}

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


	//for (int i = 0; i < corners.size(); i++) {
	//	corners[i] = getTransformation() * glm::vec4(originalCorners[i], 1);
	//}
	vDueToForce = glm::vec3(0);
	for (int i = 0; i < forces.size(); i++){
		vDueToForce += forces[i];
	}
	vDueToForce = vDueToForce * deltaT / mass;
	velocity += vDueToForce;


	for (int i = 0; i < hitboxes.size(); i++) {
		glm::mat4 full = getTransformation();
		hitboxes[i].update(&full, &translation);
	}

	if (type == "") {
		for (int k = 0; k < shaders.size(); k++) {
			shaders[k].Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaders[k].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
		}
	}

	else if (type == "A") {
		glm::mat4 bladeRotation = getTransformation()
			
			* glm::translate(glm::mat4(1), glm::vec3(0, 0, +0.65) * gScale) *
			glm::rotate(glm::mat4(1), glm::radians(bladeRotationAngle), glm::vec3(0,1,0)) * glm::translate(glm::mat4(1), glm::vec3(0,0,-0.65) * gScale);
		bladeRotationAngle += bladeRotationRate * deltaT;
			shaders[0].Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaders[0].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
			shaders[1].Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaders[1].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
			shaders[2].Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaders[2].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(bladeRotation));
			shaders[3].Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaders[3].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(bladeRotation));
		
	}
	
}

void Model::setVelocity(glm::vec3& v) {
	velocity = v;
}

void Model::setAngularVelocity(glm::vec3& v) {
	angularVelocityDirection = v;
}

void Model::scaleVelocity(float g) {
	velocity *= g;
}

void Model::setPosition(glm::vec3 given) {
	cm = given;
}

//std::vector<glm::vec3> Model::getHitboxVectors() {
//	std::vector<glm::vec3> returning;
//	for (int i = 0; i < hitboxVectors.size(); i++) {
//		returning.push_back(translation * glm::vec4(hitboxVectors[i], 1));
//	}
//	return returning;
//}

void Model::getHitboxes(Hitbox*& firstHitbox, int& size) {
	firstHitbox = &hitboxes[0];
	size = hitboxes.size();
}

int Model::getHitboxesSize() {
	return hitboxes.size();
}

//void Model::getHitboxCorners(std::vector<glm::vec3>& returning) {
//	for (int i = 0; i < corners.size(); i++) {
//		returning.push_back(corners[i]);
//	}
//}

//void Model::getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min) {
//	bool isFirst = true;
//	for (int i = 0; i < corners.size(); i++) {
//		glm::vec3 val;
//		MyMath::projection(projectVec, corners[i], val);
//		float square = MyMath::getVecMultiple(projectVec, val);
//		if (isFirst) {
//			max = square;
//			min = square;
//			isFirst = false;
//		}
//		else if (square > max) {
//			max = square;
//		}
//		else if (square < min) {
//			min = square;
//		}
//	}
//}



bool Model::load(std::string given, float scale, bool customLocation, glm::vec3 newLocation, bool addHitbox) {


	std::ifstream file(given);
	std::vector<glm::vec3> Vertices = {};
	std::vector<GLuint> Indices = {};
	int currentVertexesNumber = 0;
	int lastMaxVertexNumber = 1;

	float PosX, PosY, PosZ, NegX, NegY, NegZ;

	bool isFirst = true;

	if (file.is_open()) {
		std::istream_iterator<std::string> fileIterator(file);
		std::istream_iterator<std::string> endIterator;

		while (fileIterator != endIterator) {
			if ((*fileIterator) == "v") {

				currentVertexesNumber += 1;
				++fileIterator;
				float val1 = std::stof((*fileIterator)) * scale;
				++fileIterator;
				float val2 = std::stof((*fileIterator)) * scale;
				++fileIterator;
				float val3 = std::stof((*fileIterator)) * scale;
				if (isFirst) {
					PosX = val1;
					NegX = val1;
					PosY = val2;
					NegY = val2;
					PosZ = val3;
					NegZ = val3;
					isFirst = false;
				}
				if (val1 > PosX) {
					PosX = val1;
				}
				if (val2 > PosY) {
					PosY = val2;
				}
				if (val3 > PosZ) {
					PosZ = val3;
				}
				if (val1 < NegX) {
					NegX = val1;
				}
				if (val2 < NegY) {
					NegY = val2;
				}
				if (val3 < NegZ) {
					NegZ = val3;
				}
				Vertices.push_back(glm::vec3(val1, val2, val3));
			}
			else if ((*fileIterator) == "f") {
				int vertices[3] = { 0,0,0 };
				for (int k = 0; k < 3; k++) {
					++fileIterator;
					char curChar = ' ';
					std::string current = "";
					int i = 0;
					while (curChar != '/') {
						curChar = (*fileIterator)[i];
						current += curChar;
						i++;
					}
					vertices[k] = std::stoi(current) - lastMaxVertexNumber;
				}

				Indices.push_back(vertices[0]);
				Indices.push_back(vertices[1]);
				Indices.push_back(vertices[2]);
			}
			else if ((*fileIterator) == "o") {
				lastMaxVertexNumber = currentVertexesNumber + 1;
				mesh.push_back(Mesh(Vertices, Indices));
				Vertices.clear();
				Indices.clear();
			}


			++fileIterator;

		}
		lastMaxVertexNumber = currentVertexesNumber;
		mesh.push_back(Mesh(Vertices, Indices));
		Vertices.clear();
		Indices.clear();
		distToCenter.push_back(PosX);
		distToCenter.push_back(NegX);
		distToCenter.push_back(PosY);
		distToCenter.push_back(NegY);
		distToCenter.push_back(PosZ);
		distToCenter.push_back(NegZ);


		std::vector < glm::vec3> originalCorners;

		originalCorners.push_back(glm::vec3(PosX, PosY, PosZ));
		originalCorners.push_back(glm::vec3(PosX, PosY, NegZ));
		originalCorners.push_back(glm::vec3(PosX, NegY, PosZ));
		originalCorners.push_back(glm::vec3(PosX, NegY, NegZ));
		originalCorners.push_back(glm::vec3(NegX, PosY, PosZ));
		originalCorners.push_back(glm::vec3(NegX, PosY, NegZ));
		originalCorners.push_back(glm::vec3(NegX, NegY, PosZ));
		originalCorners.push_back(glm::vec3(NegX, NegY, NegZ));

		if (addHitbox) {

			//for (int i = 0; i < originalCorners.size(); i++) {
			//	corners.push_back(originalCorners[i]);
			//}


			std::vector<glm::vec3> hitboxVectors;
			hitboxVectors.push_back(glm::vec3(1, 0, 0));
			hitboxVectors.push_back(glm::vec3(0, 1, 0));
			hitboxVectors.push_back(glm::vec3(0, 0, 1));

			std::vector<int> edges = { 0,1,2,3,4,5,6,7,0,4,0,2,1,5,1,3,2,6,3,7,4,6,5,7 };
			Hitbox h = Hitbox(originalCorners, hitboxVectors, this, edges);
			hitboxes.push_back(h);
		}




		glm::vec3 currentSum = originalCorners[0];
		for (int i = 1; i < originalCorners.size(); i++) {
			currentSum += originalCorners[i];
		}
		cm = currentSum / (float)originalCorners.size();
		originalCm = cm;
		if (customLocation) {
			cm = newLocation;

		}

		file.close();


	}

	return 0;
}

bool Model::loadHitbox(std::string given, float scale, glm::vec3 newLocation) {


	std::ifstream file(given);

	float PosX, PosY, PosZ, NegX, NegY, NegZ
		, PosXTotal, PosYTotal, PosZTotal, NegXTotal, NegYTotal, NegZTotal;

	bool isFirst = true;
	bool shouldScan = true;


	if (file.is_open()) {
		std::istream_iterator<std::string> fileIterator(file);
		std::istream_iterator<std::string> endIterator;

		while (fileIterator != endIterator) {
			if ((*fileIterator) == "v") {

				++fileIterator;
				float val1 = std::stof((*fileIterator)) * scale;
				++fileIterator;
				float val2 = std::stof((*fileIterator)) * scale;
				++fileIterator;
				float val3 = std::stof((*fileIterator)) * scale;
				if (shouldScan) {
					PosX = val1;
					NegX = val1;
					PosY = val2;
					NegY = val2;
					PosZ = val3;
					NegZ = val3;
					shouldScan = false;
				}
				if (isFirst) {
					PosXTotal = val1;
					NegXTotal = val1;
					PosYTotal = val2;
					NegYTotal = val2;
					PosZTotal = val3;
					NegZTotal = val3;
					isFirst = false;
				}
				if (val1 > PosX) {
					PosX = val1;
				}
				if (val2 > PosY) {
					PosY = val2;
				}
				if (val3 > PosZ) {
					PosZ = val3;
				}
				if (val1 < NegX) {
					NegX = val1;
				}
				if (val2 < NegY) {
					NegY = val2;
				}
				if (val3 < NegZ) {
					NegZ = val3;
				}

				if (val1 > PosXTotal) {
					PosXTotal = val1;
				}
				if (val2 > PosYTotal) {
					PosYTotal = val2;
				}
				if (val3 > PosZTotal) {
					PosZTotal = val3;
				}
				if (val1 < NegXTotal) {
					NegXTotal = val1;
				}
				if (val2 < NegYTotal) {
					NegYTotal = val2;
				}
				if (val3 < NegZTotal) {
					NegZTotal = val3;
				}
			}
			else if ((*fileIterator) == "o") {
				createHitbox(PosX, PosY, PosZ, NegX, NegY, NegZ);
				shouldScan = true;
			}




			++fileIterator;

		}
		createHitbox(PosX, PosY, PosZ, NegX, NegY, NegZ);

		cm = glm::vec3((PosXTotal + NegXTotal) / 2.0f, (PosYTotal + NegYTotal) / 2.0f, (PosZTotal + NegZTotal) / 2.0f);
		originalCm = cm;

		cm = newLocation;



		file.close();


	}
	return 0;
}

void Model::createHitbox(float PosX, float PosY, float PosZ, float NegX, float NegY, float NegZ) {
	std::vector < glm::vec3> originalCorners;

	originalCorners.push_back(glm::vec3(PosX, PosY, PosZ));
	originalCorners.push_back(glm::vec3(PosX, PosY, NegZ));
	originalCorners.push_back(glm::vec3(PosX, NegY, PosZ));
	originalCorners.push_back(glm::vec3(PosX, NegY, NegZ));
	originalCorners.push_back(glm::vec3(NegX, PosY, PosZ));
	originalCorners.push_back(glm::vec3(NegX, PosY, NegZ));
	originalCorners.push_back(glm::vec3(NegX, NegY, PosZ));
	originalCorners.push_back(glm::vec3(NegX, NegY, NegZ));
	//for (int i = 0; i < originalCorners.size(); i++) {
	//	corners.push_back(originalCorners[i]);
	//}


	std::vector<glm::vec3> hitboxVectors;
	hitboxVectors.push_back(glm::vec3(1, 0, 0));
	hitboxVectors.push_back(glm::vec3(0, 1, 0));
	hitboxVectors.push_back(glm::vec3(0, 0, 1));

	std::vector<int> edges = { 0,1,2,3,4,5,6,7,0,4,0,2,1,5,1,3,2,6,3,7,4,6,5,7 };
	Hitbox h = Hitbox(originalCorners, hitboxVectors, this, edges);
	hitboxes.push_back(h);
}


void Model::rotate(float angle, glm::vec3& norm) {
	glm::mat4 rot = glm::mat4(1);
	rot = glm::rotate(rot, glm::radians(angle), norm);
	translation = rot * translation;
	inverseTranslation = glm::inverse(translation);
}

void Model::moveBy(glm::vec3& given) {
	cm += given;
}

float Model::getMass() {
	return mass;
}

glm::vec3* Model::getCm() {
	return &cm;
}

void Model::addAngularVelocity(glm::vec3& given) {
	angularVelocityDirection += given;
}

float Model::getMagnitudeVelocity() {
	return sqrt(MyMath::getVectorMagnitudeSquared(velocity));
}

glm::vec3 Model::getVelocity() {
	return velocity;
}

void Model::addVelocity(glm::vec3& given) {
	velocity += given;
}

glm::vec3 Model::getFacing() {
	return  translation * glm::vec4(facing, 1);
}

bool Model::isMovable() {
	return movable;
}


void Model::setupCybertruck(float scale) {

	std::vector<glm::vec3> frontLeftWheel;
	std::vector<glm::vec3> frontRightWheel;
	std::vector<glm::vec3> rearRightWheel;
	std::vector<glm::vec3> rearLeftWheel;
	std::vector<glm::vec3> body;
	std::vector<glm::vec3> wheelNormal;
	std::vector<glm::vec3> bodyNormal;




	frontRightWheel.push_back(glm::vec3(-1.068526, -0.024803, 2.109123) * scale);
	frontRightWheel.push_back(glm::vec3(-1.068526, 0.377191, 2.456192) * scale);
	frontRightWheel.push_back(glm::vec3(-1.068526, -0.024803, 1.828386) * scale);
	frontRightWheel.push_back(glm::vec3(-1.068526, 0.377191, 1.481317) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, -0.024803, 2.109123) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, 0.377191, 2.456192) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, -0.024803, 1.828386) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, 0.377191, 1.481317) * scale);

	frontLeftWheel.push_back(glm::vec3(0.686718, -0.024803, 2.109123) * scale);
	frontLeftWheel.push_back(glm::vec3(0.686718, 0.377191, 2.456192) * scale);
	frontLeftWheel.push_back(glm::vec3(0.686718, -0.024803, 1.828386) * scale);
	frontLeftWheel.push_back(glm::vec3(0.686718, 0.377191, 1.481317) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, -0.024803, 2.109123) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, 0.377191, 2.456192) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, -0.024803, 1.828386) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, 0.377191, 1.481317) * scale);

	rearLeftWheel.push_back(glm::vec3(0.686718, -0.024803, -1.618060) * scale);
	rearLeftWheel.push_back(glm::vec3(0.686718, 0.377191, -1.270991) * scale);
	rearLeftWheel.push_back(glm::vec3(0.686718, -0.024803, -1.898797) * scale);
	rearLeftWheel.push_back(glm::vec3(0.686718, 0.377191, -2.245865) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, -0.024803, -1.618060) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, 0.377191, -1.270991) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, -0.024803, -1.898797) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, 0.377191, -2.245865) * scale);

	rearRightWheel.push_back(glm::vec3(-1.068526, -0.024803, -1.618060) * scale);
	rearRightWheel.push_back(glm::vec3(-1.068526, 0.377191, -1.270991) * scale);
	rearRightWheel.push_back(glm::vec3(-1.068526, -0.024803, -1.898797) * scale);
	rearRightWheel.push_back(glm::vec3(-1.068526, 0.377191, -2.245865) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, -0.024803, -1.618060) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, 0.377191, -1.270991) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, -0.024803, -1.898797) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, 0.377191, -2.245865) * scale);

	body.push_back(glm::vec3(-1.068788, 0.364160, 2.884884) * scale);
	body.push_back(glm::vec3(-1.068788, 1.871332, 0.464945) * scale);
	body.push_back(glm::vec3(-1.068788, 0.364160, -2.808581) * scale);
	body.push_back(glm::vec3(-1.068788, 1.871332, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 0.364160, 2.884884) * scale);
	body.push_back(glm::vec3(1.059664, 1.871332, 0.464945) * scale);
	body.push_back(glm::vec3(1.059664, 0.364160, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 1.871332, -2.808581) * scale);
	body.push_back(glm::vec3(-1.068788, 1.021972, 2.884884) * scale);
	body.push_back(glm::vec3(-1.068788, 1.021972, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 1.021972, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 1.021972, 2.884884) * scale);

	wheelNormal.push_back(glm::vec3(0, 1, 0));
	wheelNormal.push_back(glm::vec3(1, 0, 0));
	wheelNormal.push_back(glm::vec3(0, -1 / sqrt(2), 1 / sqrt(2)));
	wheelNormal.push_back(glm::vec3(0, -1 / sqrt(2), -1 / sqrt(2)));


	bodyNormal.push_back(glm::vec3(0, 1, 0));
	bodyNormal.push_back(glm::vec3(1, 0, 0));
	bodyNormal.push_back(glm::vec3(0, 0, 1));
	bodyNormal.push_back(glm::vec3(0, 1 / sqrt(8.1175480847), 2.84913111048 / sqrt(8.1175480847)));
}



Hitbox* Model::getHitbox(int i) {
	return &hitboxes[i];
}


void Model::changeIsMovable(bool newValue) {
	movable = newValue;
}

void Model::addImpulse(Impulse imp) {
	impulses.push_back(imp);
}

float Model::getBounceFactor() {
	return bounceFactor;
}

float Model::getFrictionFactor() {
	return frictionFactor;
}

glm::vec3* Model::getAngularVelocity() {
	return &angularVelocityDirection;
}

void Model::dealWithImpulses() {
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
		if (MyMath::getVectorMagnitudeSquared(vUpdate) > 0.75 || MyMath::getVectorMagnitudeSquared(velocity)>0.9 || 
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


void Model::setMass(float gMass) {
	for (int i = 0; i < forces.size(); i++) {
		forces[i] = forces[i] / mass * gMass;
	}
	mass = gMass;
}

void Model::accelerateForward(float ammount, float deltaT) {
	velocity += glm::normalize(getFacing()) * ammount * deltaT;
}

void Model::driveRotate(float degrees, float deltaT) {
	if (currentlyFlying) {
		rotate(-degrees * deltaT * 100 / 7, localY);
	}
	else if (currentlyDriving) {
		rotate(-degrees * glm::length(velocity) * deltaT / 7, localY);

	}
	velocity = glm::rotate(velocity, glm::radians(-degrees * glm::length(velocity) * deltaT/7), localY);
}

void Model::pitchRotate(float degrees, float deltaT) {
	if (currentlyFlying) {
		rotate(-degrees * deltaT * 100 / 7, localX);
	}
}

void Model::driveRotateX(float degrees, float deltaT) {
	rotate(-degrees * glm::length(velocity) * deltaT / 7, localX);
}

void Model::driveRollX(float degrees, float deltaT) {
	rotate(-degrees * glm::length(velocity) * deltaT / 7, localZ);
}

bool Model::impulsesEmpty() {
	return impulses.size() == 0;
}


glm::vec3 Model::applyInverseInertiaTensor(glm::vec3 cmToImpact, glm::vec3 normal) {
	//glm::vec3 impactToCm = pos - cm;
	glm::vec3 inOriginalCoordinates = inverseTranslation * glm::vec4(cmToImpact, 1.0f);
	glm::vec3 translatedNormal = inverseTranslation * glm::vec4(normal, 1.0f);
	glm::vec3 tensored = inverseInertiaTensor * glm::cross(inOriginalCoordinates, translatedNormal);
	glm::vec3 tensoerdInOriginal = translation * glm::vec4(tensored, 1.0f);
	return tensoerdInOriginal;
}

glm::mat4 Model::getInverseTransformation() {
	return inverseTranslation;
}

glm::mat4* Model::getRawTransformation() {
	return &translation;
}

void Model::addDebugger(Model * given) {
	debugger.push_back(given);
}

glm::vec3 Model::velocityAtPoint(glm::vec3& atPoint) {
	return velocity + glm::cross(angularVelocityDirection, atPoint - cm);
}

void Model::addForce(glm::vec3 force) {
	forces.push_back(force);
}

glm::vec3 Model::getForces() {
	glm::vec3 force = glm::vec3(0);
	for (int i = 0; i < forces.size(); i++) {
		force += forces[i];
	}
	return force;
}

void Model::setCurrentlyDriving(bool given) {
	currentlyDriving = given;
	//if (given) {
	//	frictionFactor = 0.7;

	//}
	//else {
	//	0.7;
	//}
}

bool Model::getCurrentlyDriving() {
	return currentlyDriving;
}

bool Model::getCurrentlyFlying() {
	return currentlyFlying;
}

void Model::setCurrentlyFlying(bool given) {
	currentlyFlying = given;
}

void Model::setWantedRotation(float wanted) {
	wantedSteeringPos = wanted;
}


void Model::setWantedVerticalRotation(float wanted) {
	wantedVerticalSteeringPos = wanted;
}

void Model::setWantedRollRotation(float wanted) {
	wantedRollSteeringPos = wanted;
}

void Model::addFrictionForce(Friction gImp) {
	frictions.push_back(gImp);
}

void Model::dealWithFrictionForce() {
	if (frictionShouldApply) {
		for (int i = 0; i < frictions.size(); i++) {
			if (glm::length(frictions[i].value) != 0) {
				glm::vec3 cmToImpulse = frictions[i].position - cm;

				glm::vec3 cmToCmImpulseTranslated = inverseTranslation * glm::vec4(cmToImpulse, 1.0f);
				glm::vec3 translatedImpulse = inverseTranslation * glm::vec4(frictions[i].value, 1.0f);
				glm::vec3 changeInAngular = inverseInertiaTensor * glm::cross(cmToCmImpulseTranslated, translatedImpulse);
				glm::vec3 changeInAngularInOriginal = translation * glm::vec4(changeInAngular, 1.0f);

				glm::vec3 newV = velocity + frictions[i].value / mass;
				glm::vec3 newAV = angularVelocityDirection + changeInAngularInOriginal;

				if (glm::dot(newV + glm::cross(newAV, cmToImpulse), frictions[i].value) > 0 && !frictions[i].otherModel->isMoving()) {
					//std::cout << "@@@";
				}
				else {
					//std::cout << "!!!";
					velocity = newV;
					angularVelocityDirection = newAV;
				}
			}
		}
	}
	frictions.clear();
}

bool Model::isMoving() {
	return MyMath::getVectorMagnitudeSquared(velocity) > 0.5; //friction !!!
}

void Model::loadAppache(float gmass) {
	localY = glm::vec3(0, 1, 0);
	localX = glm::vec3(1, 0, 0);
	localZ = glm::vec3(0, 0, 1);
	mass = gmass;
	translation = glm::mat4(1);
	inverseTranslation = glm::mat4(1);
	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(0, 0, 0);
	angularVelocity = 0;
	facing = glm::vec3(0, 0, 1);
	ShaderClass shaderProgram("default.vert", "default.geom", "default.frag");
	ShaderClass shaderProgram2("default.vert", "outline.geom", "outline.frag");
	ShaderClass bladesShader("default.vert", "default.geom", "default.frag");
	ShaderClass bladesShader2("default.vert", "outline.geom", "outline.frag");

	shaders.push_back(shaderProgram);
	shaders.push_back(shaderProgram2);
	shaders.push_back(bladesShader);
	shaders.push_back(bladesShader2);

	for (int k = 0; k < shaders.size(); k++) {
		shaders[k].Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaders[k].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
	}


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


