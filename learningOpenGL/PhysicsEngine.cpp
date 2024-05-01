#include "PhysicsEngine.h"



void PhysicsEngine::generateAxis_Hitboxes(Hitbox& hitbox1, Hitbox& hitbox2, std::vector<glm::vec3>& returning) {
	std::vector<glm::vec3> l1 = hitbox1.getNormalVectors();
	std::vector<glm::vec3> l2 = hitbox2.getNormalVectors();
	for (int i = 0; i < l1.size(); i++) {
		returning.push_back(l1[i]);
	}

	for (int i = 0; i < l2.size(); i++) {
		returning.push_back(l2[i]);
	}

	for (int i = 0; i < l1.size(); i++) {
		for (int k = 0; k < l2.size(); k++) {
			glm::vec3 appending;
			MyMath::vectorCross(l1[i], l2[k], appending);
			if (appending.x != 0 || appending.y != 0 || appending.z != 0) {
				returning.push_back(appending);

			}
		}
	}
}


bool PhysicsEngine::checkHitboxes(Model& model1, Model& model2, IntersectionModel& given) {
	//float curDistance = 0;
	bool collidingWhole = false;
	bool isFirst = true;
	given.solutionVec = glm::vec3(0);
	for (int i = 0; i < model1.getHitboxesSize(); i++) {
		for (int k = 0; k < model2.getHitboxesSize(); k++) {

			float intersectionDistance = -1.0f;
			glm::vec3 normalCollision = glm::vec3(0);
			bool colliding = checkHitboxesColliding((*model1.getHitbox(i)), (*model2.getHitbox(k)), intersectionDistance, normalCollision);
			if (isFirst && colliding) {
				//curDistance = intersectionDistance;
				given.solutionVec = normalCollision * intersectionDistance;
				isFirst = false;
			}
			if (colliding) {
				if (abs(given.solutionVec.x) <= abs(normalCollision.x * intersectionDistance)) {
					given.solutionVec.x = normalCollision.x * intersectionDistance;
				}
				if (abs(given.solutionVec.y) <= abs(normalCollision.y * intersectionDistance)) {
					given.solutionVec.y = normalCollision.y * intersectionDistance;
				}
				if (abs(given.solutionVec.z) <= abs(normalCollision.z * intersectionDistance)) {
					given.solutionVec.z = normalCollision.z * intersectionDistance;
				}


				given.model1 = &model1;
				given.model2 = &model2;
				//given.solutionVec = normalCollision;
				//given.amountIntersect = intersectionDistance;
				//curDistance = intersectionDistance;


				collidingWhole = true;
			}
		}
	}



	return collidingWhole;
}





void PhysicsEngine::detectCollisions() {
	std::vector<int> modelsToCheck;
	comb(models->size(), 2, modelsToCheck);
	for (int i = 0; i < modelsToCheck.size(); i += 2) {
		glm::vec3 normal;
		float intersectBy;
		IntersectionModel model;
		bool colliding = checkHitboxes(*(*models)[modelsToCheck[i]], *(*models)[modelsToCheck[i + 1]], model);
		if (colliding) {
			currentCollisions.push_back(model);
		}
	}
}



void PhysicsEngine::dealWithCollisions() {
	for (int i = 0; i < currentCollisions.size(); i++) {

		if (currentCollisions[i].model1->isMovable() && currentCollisions[i].model2->isMovable()) {
			dealWithBothMovable(i);
		}
		else if (currentCollisions[i].model1->isMovable()) {
			dealWithFirstMovable(i);
		}
		else if (currentCollisions[i].model2->isMovable()) {
			dealWithSecondMovable(i);
		}
	}
	currentCollisions.clear();
}


glm::vec3 PhysicsEngine::calculateTVec(Model& m1, Model& m2, glm::vec3& pos, glm::vec3& normal) {
	glm::vec3 vr = m1.velocityAtPoint(pos) - m2.velocityAtPoint(pos);

	if (glm::dot(vr, normal) != 0) {
		glm::vec3 case1 = vr - normal * glm::dot(vr, normal);
		return case1;
	}
}

void PhysicsEngine::applyDrag(float dt) {
	for (int i = 0; i < (*models).size(); i++) {
		if ((*models)[i]->isMovable()) {
			(*models)[i]->scaleVelocity(pow(dragConstant, -dt));
		}
	}
}





void PhysicsEngine::addImpulses(Model& model1, Model& model2, std::vector<Contact>& vec) {

	glm::vec3 centerCollision = glm::vec3(0, 0, 0);
	for (int i = 0; i < vec.size(); i++) {
		//vpc::printVec(vec[i].position);
		centerCollision += vec[i].position;
	}
	centerCollision = centerCollision / (float)vec.size();
	//vpc::printVec(centerCollision);
	//std::cout << "\n \n ====== \n";
	glm::vec3 frictionAmount = glm::vec3(0, 0, 0);

	for (int i = 0; i < vec.size(); i++) {
		glm::vec3 cm1ToCollision = centerCollision - *model1.getCm();
		glm::vec3 cm2ToCollision = centerCollision - *model2.getCm();
		float bounceFactor = model1.getBounceFactor() * model2.getBounceFactor();
		float frictionFactor = model1.getFrictionFactor() * model2.getFrictionFactor();
		glm::vec3 vDiff = model1.getVelocity() - model2.getVelocity();
		glm::vec3 r1CrossN = glm::cross(cm1ToCollision, vec[i].normal);
		glm::vec3 r2CrossN = glm::cross(cm2ToCollision, vec[i].normal);
		float massFactor = 0;
		if (model1.isMovable() && model2.isMovable()) {
			massFactor = (1 / model1.getMass() + 1 / model2.getMass());
		}
		else if (model1.isMovable()) {
			massFactor = 1 / model1.getMass();
		}
		else if (model2.isMovable()) {
			massFactor = 1 / model2.getMass();
		}
		glm::vec3 J =
			vec[i].normal * (1 + bounceFactor) *
			(glm::dot(vDiff, vec[i].normal)
				+ glm::dot(r1CrossN, *model1.getAngularVelocity())
				- glm::dot(r2CrossN, *model2.getAngularVelocity())) /
			// --------------------------------------------------------------------
			(massFactor
				+ (model1.isMovable() ? glm::dot(r1CrossN, model1.applyInverseInertiaTensor(cm1ToCollision, vec[i].normal)) : 0)
				+ (model2.isMovable() ? glm::dot(r2CrossN, model2.applyInverseInertiaTensor(cm2ToCollision, vec[i].normal)) : 0)) / (float)vec.size();
		//vpc::printVec(cm2ToCollision);
		//std::cout << glm::dot(r1CrossN, *model1.getAngularVelocity()) << "\n";

		Impulse impOn2;
		impOn2.direction = J * (1.0f);
		//impOn2.directionFriction = projVonNormal;
		impOn2.position = vec[i].position;
		impOn2.centerCollision = centerCollision;
		Impulse impOn1;
		impOn1.direction = J * (-1.0f);
		//impOn1.directionFriction = impOn2.directionFriction * (-1.0f);
		impOn1.position = vec[i].position;
		impOn1.centerCollision = centerCollision;

		glm::vec3 velocityDifferences = model1.velocityAtPoint(vec[i].position) - model2.velocityAtPoint(vec[i].position);


		glm::vec3 frictionDir = velocityDifferences - glm::dot(velocityDifferences, vec[i].normal) * vec[i].normal;
		//std::cout << glm::length(frictionDir) << " friction dir \n";
		if (glm::length(frictionDir) > 0.5) {
			frictionDir = glm::normalize(frictionDir);
			//vpc::printVec(velocityDifferences);
			//vpc::printVec(vec[i].normal);
			glm::vec3 frictionOn2 = frictionDir * (float)glm::length(J) * frictionFactor;
			frictionAmount += frictionOn2;
			//vpc::printVec(frictionOn2);
		}

		if (model1.isMovable()) {
			model1.addImpulse(impOn1);
		}
		if (model2.isMovable()) {
			model2.addImpulse(impOn2);
		}
	}
	//vpc::printVec(frictionAmount);
	//std::cout << "=======\n";

	Friction fricOn2;
	fricOn2.position = centerCollision;
	fricOn2.value = frictionAmount;
	fricOn2.otherModel = &model1;
	Friction fricOn1;
	fricOn1.position = centerCollision;
	fricOn1.value = frictionAmount * (-1.0f);
	fricOn1.otherModel = &model2;
	//std::cout << model1.getVelocity().y << "\n";
	if (model1.isMovable()) {
		if (model1.getCurrentlyDriving()) {
			//std::cout << "ran1 \n";
			fricOn1.value = (fricOn1.value - glm::dot(fricOn1.value, model1.getFacing()) * model1.getFacing());
		}
		model1.addFrictionForce(fricOn1);
	}
	if (model2.isMovable()) {
		if (model2.getCurrentlyDriving()) {
			//std::cout << "ran2 \n";
			fricOn2.value = (fricOn2.value - glm::dot(fricOn2.value, model2.getFacing()) * model2.getFacing());
		}
		model2.addFrictionForce(fricOn2);
	}
}



glm::vec3 PhysicsEngine::getClosestPointsOnLines(glm::vec3& a0, glm::vec3& b0, glm::vec3& a1, glm::vec3& b1, bool& worked) {
	glm::vec3 v1 = b0 - a0;
	glm::vec3 v2 = b1 - a1;
	glm::vec3 cross = glm::cross(v1, v2);
	if (MyMath::getVectorMagnitudeSquared(cross) < 0.001) {
		worked = false;
		return v1;
	}
	float t = glm::dot(glm::cross((a1 - a0), v2), cross) / MyMath::getVectorMagnitudeSquared(cross);
	worked = t < 0 ? false : t>1 ? false : true;
	return a0 + t * v1;
}



void PhysicsEngine::correctNormalsToPointRightDirection(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
	for (int i = 0; i < given.size(); i++) {
		glm::vec3 cmToPos = given[i].position - *h1.getCm();
		if (glm::dot(cmToPos, given[i].normal) < 0) {
			given[i].normal = given[i].normal * (-1.0f);
		}
	}
}




void PhysicsEngine::detectEdgeEdge(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
	for (int i = 0; i < h1.getEdgeNumber(); i++) {
		for (int k = 0; k < h2.getEdgeNumber(); k++) {
			glm::vec3 a0, b0, a1, b1;
			h1.getEdge(i, a0, b0);
			h2.getEdge(k, a1, b1);
			bool worked1, worked2;
			glm::vec3 onh1 = getClosestPointsOnLines(a0, b0, a1, b1, worked1);
			glm::vec3 onh2 = getClosestPointsOnLines(a1, b1, a0, b0, worked2);

			glm::vec3 h1CmToOnH1 = *h1.getCm() - onh1;
			glm::vec3 h1CmToOnH2 = *h1.getCm() - onh2;

			glm::vec3 h2CmToOnH1 = *h2.getCm() - onh1;
			glm::vec3 h2CmToOnH2 = *h2.getCm() - onh2;

			glm::vec3 h1ToPointA = onh1 - a0;
			glm::vec3 h1ToPointB = onh1 - b0;
			glm::vec3 h2ToPointA = onh2 - a1;
			glm::vec3 h2ToPointB = onh2 - b1;

			glm::vec3 onh1ToOnH2 = onh2 - onh1;
			if (worked1 && worked2 &&
				MyMath::getVectorMagnitudeSquared(h1CmToOnH1) > MyMath::getVectorMagnitudeSquared(h1CmToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h2CmToOnH1) < MyMath::getVectorMagnitudeSquared(h2CmToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h1ToPointA) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h1ToPointB) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h2ToPointA) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h2ToPointB) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2)
				) {
				glm::vec3 appending = (onh1 + onh2) / 2.0f;
				//std::cout << "\n\n this Ran " << onh1.x << " " << onh1.y << " " << onh1.z << " on vertices: \n =====================\n" 
				//	<< a0.x << " " << a0.y << " " << a0.z << "\n" << b0.x << " " << b0.y << " " << b0.z << "\n\n" 
				//	<< a1.x << " " << a1.y << " " << a1.z << "\n" << b1.x << " " << b1.y << " " << b1.z <<  " "<< "\n" << i << " " << k << "\n======================\n";
				//std::cout << "this Ran  " << onh2.x << " " << onh2.y << " " << onh2.z << "\n";
				//std::cout << i << " " << k << "\n";




				Contact contact;
				contact.position = appending;
				glm::vec3 normal = glm::normalize(glm::cross(b0 - a0, b1 - a1));
				if (glm::dot(appending - *h1.getCm(), normal) < 0) {
					normal = normal * (-1.0f);
				}
				contact.normal = normal;
				given.push_back(contact);
			}
		}
	}
}





void PhysicsEngine::detectPointFace(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
	std::vector<glm::vec3* > v1;
	std::map<glm::vec3*, float> valsFloat;
	std::map<glm::vec3*, glm::vec3> valsVector;
	for (int i = 0; i < h2.getVectorsSize(); i++) {
		v1.push_back(h2.getVec(i));
		valsFloat.insert({ h2.getVec(i) , 100 });
		valsVector.insert({ h2.getVec(i), glm::vec3(0) });
	}


	for (int i = 0; i < h1.getNormalsSize(); i++) {
		glm::vec3 projectedCm = glm::vec3(0);
		MyMath::projection(*h1.getNormal(i), (*h1.getCm()), projectedCm);
		float cm = MyMath::getVecMultiple(*h1.getNormal(i), projectedCm);
		float max, min;
		h1.getMaxMin(i, cm, max, min);
		//std::cout << cm << " " << max << " " << min << "\n";


		for (int k = 0; k < v1.size(); k++) {
			glm::vec3 v;
			MyMath::projection(*h1.getNormal(i), *v1[k], v);
			float multiple = MyMath::getVecMultiple(*h1.getNormal(i), v);
			if (multiple >= max || multiple <= min) {
				v1.erase(v1.begin() + k);
				k--;
			}
			else {
				if (multiple < max && max - multiple < valsFloat[v1[k]]) {
					valsFloat[v1[k]] = max - multiple;
					valsVector[v1[k]] = *h1.getNormal(i);
				}
				if (multiple > min && multiple - min < valsFloat[v1[k]]) {
					valsFloat[v1[k]] = multiple - min;
					valsVector[v1[k]] = *h1.getNormal(i);
				}
			}
		}
	}
	for (int i = 0; i < v1.size(); i++) {
		Contact contact;
		contact.position = *v1[i];
		contact.normal = valsVector[v1[i]];
		given.push_back(contact);
	}
}




void PhysicsEngine::comb(int N, int K, std::vector<int>& returning) {
	std::string bitmask(K, 1); // K leading 1's
	bitmask.resize(N, 0); // N-K trailing 0's

	// print integers and permute bitmask
	do {
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) returning.push_back(i);
		}
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}




bool PhysicsEngine::checkHitboxesColliding(Hitbox& hitbox1, Hitbox& hitbox2, float& curintersect, glm::vec3& normalToIntersect) {
	std::vector<glm::vec3> axis;
	generateAxis_Hitboxes(hitbox1, hitbox2, axis);
	bool firstCheck = true;

	for (int i = 0; i < axis.size(); i++) {
		float mod1Max, mod1Min, mod2Max, mod2Min;
		hitbox1.getMaxMinFromProjection(axis[i], mod1Max, mod1Min);
		hitbox2.getMaxMinFromProjection(axis[i], mod2Max, mod2Min);
		//std::cout << mod1Max << " " << mod1Min << " " << mod2Max << " " << mod2Min << "\n";
		if (mod1Max <= mod2Min || mod1Min >= mod2Max) {
			return false;
		}
		if (firstCheck) {
			if (mod1Max > mod2Min) {
				curintersect = mod1Max - mod2Min;
			}
			else {
				curintersect = mod1Min - mod2Max;
			}
			normalToIntersect = axis[i];
			firstCheck = false;
		}
		else if (mod1Max >= mod2Min && abs(curintersect) > abs(mod1Max - mod2Min)) {
			//std::cout << "ran option 1 \n";
			curintersect = mod1Max - mod2Min;
			normalToIntersect = axis[i];
		}
		else if (mod1Min <= mod2Max && abs(curintersect) > abs(mod2Max - mod1Min)) {
			//std::cout << "ran option 2 \n";

			curintersect = mod1Min - mod2Max;
			normalToIntersect = axis[i];
		}
	}
	//std::cout << curintersect << "\n";

	return true;
}





void PhysicsEngine::dealWithFirstMovable(int i) {
	glm::vec3 moveBy1 = currentCollisions[i].solutionVec * (-1.0f);


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);

	currentCollisions[i].model1->moveBy(moveBy1);
}



void PhysicsEngine::dealWithBothMovable(int i) {


	glm::vec3 moveBy2 = currentCollisions[i].solutionVec / 2.0f;
	glm::vec3 moveBy1 = moveBy2 * (-1.0f);


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);



	currentCollisions[i].model1->moveBy(moveBy1);
	currentCollisions[i].model2->moveBy(moveBy2);
}



void PhysicsEngine::dealWithSecondMovable(int i) {
	glm::vec3 moveBy2 = currentCollisions[i].solutionVec * 1.0f;


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	currentCollisions[i].model2->moveBy(moveBy2);
}






void PhysicsEngine::generateContacts(Model& model1, Model& model2, std::vector <Contact>& given) {
	for (int i = 0; i < model1.getHitboxesSize(); i++) {
		for (int k = 0; k < model2.getHitboxesSize(); k++) {
			detectPointFace(*model1.getHitbox(i), *model2.getHitbox(k), given);
			detectPointFace(*model2.getHitbox(k), *model1.getHitbox(i), given);
			detectEdgeEdge(*model1.getHitbox(i), *model2.getHitbox(k), given);
			correctNormalsToPointRightDirection(*model1.getHitbox(i), *model2.getHitbox(k), given);
		}
	}
}


void PhysicsEngine::update(float deltaT) {
	for (int i = 0; i < (*models).size(); i++) {
		(*models)[i]->update(deltaT);
	}
}


void PhysicsEngine::applyDealWithImpulses() {
	for (int i = 0; i < (*models).size(); i++) {
		(*models)[i]->dealWithImpulses();
		(*models)[i]->dealWithFrictionForce();
	}
}


void PhysicsEngine::updateEngine(float dt) {
	update(dt);
	detectCollisions();
	dealWithCollisions();
	applyDealWithImpulses();
	applyDrag(dt);
}

PhysicsEngine::PhysicsEngine(std::vector < Model*> * gModels) {
	models = gModels;
}

PhysicsEngine::PhysicsEngine() {

}

void PhysicsEngine::setVector(std::vector<Model* > * gModels) {
	models = gModels;
}