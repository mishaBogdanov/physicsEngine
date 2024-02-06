#include "World.h"
#include <thread>
#include <iostream>
#include <windows.h>
#include "MyMath.h"
#include "Model.h"
#include "Hitbox.h"
#include "vectorPrintingClass.h"
#include <map>
#include "Helicopter.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);

World::World(float distX, float distY, float distZ, int divs, bool gravity) {
	sizeX = distX;
	sizeY = distY;
	sizeZ = distZ;
	divisions = divs;
	gravityEnabled = gravity;
	models.reserve(200);
	helicopters.reserve(30);
	explosionModels.reserve(20);
}



void World::setupGLFW() {
	// glfw: initialize and configure
		// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	//GLFW.glfwWindowHint(GLFW.GLFW_VISIBLE, GLFW.GLFW_FALSE);
	//if (isConsole) {
	//	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	//}

	//glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);



	time = glfwGetTime();
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	dwBytesWritten = 0;
	SetConsoleActiveScreenBuffer(hConsole);

}

void World::addFloor(int gNum, float gSpacing, float gHeight, float gz, float gx) {
	floors.push_back(Floor(gNum, gSpacing, gHeight, gz, gx));

}

void World::addModel(std::string given) {
	Model* setModel = new Model(given);
	addModel(setModel);
}

void World::addModel(Model* m) {
	models.push_back(m);
}

void World::addModel(std::string given, float scale) {
	Model* setModel = new Model(given, scale);
	addModel(setModel);
}

void World::addHitbox(std::string given, float scale, glm::vec3 location) {
	Model * m = new Model(given, scale, location, 2);
	addModel(m);
}

void World::addVehichle(std::string given, float scale, glm::vec3 location) {
	if (given == "Apache.object") {
		Model* m = new Model(given, scale, location, 1);
		m->addForce(gravityAcceleration * m->getMass());
		addModel(m);
		Helicopter apache = Helicopter(m->getMass(), m, this);
		helicopters.push_back(apache);
	}
}


void World::addModel(std::string given, float scale, glm::vec3 pos, bool hitbox, bool movable) {

	Model *m = new Model(given, scale, pos, hitbox ? 1 : 3);

	if (movable && gravityEnabled) {
		m->addForce(gravityAcceleration * m->getMass());
	}
	else if (!movable) {
		m->changeIsMovable(false);
	}
	addModel(m);
}

void World::setVelocity(int location, glm::vec3 v) {
	models[location]->setVelocity(v);
}

void World::setAngularVelocity(int location, glm::vec3 v) {
	models[location]->setAngularVelocity(v);
}

void World::setMass(int index, float mass) {
	models[index]->setMass(mass);
}




void World::rotateModel(int location, float angle, glm::vec3& norm) {
	models[location]->rotate(angle, norm);
}



//void World::screenToPixel() {
//
//	if (isConsole) {
//
//		glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_FLOAT, pixels);
//
//		for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT * 3; i += 3) {
//
//			int pos = 0;
//			if (i / 3 / SCR_WIDTH == 0) {
//				pos = (SCR_HEIGHT - 1) * SCR_WIDTH + i / 3;
//				//std::cout << i / 3;
//			}
//			else {
//				pos = (SCR_HEIGHT - (i / 3 / SCR_WIDTH) - 1) * SCR_WIDTH + ((i / 3) % SCR_WIDTH);
//				//std::cout << pos;
//			}
//
//
//			float colorValue = pixels[i] + pixels[i + 1] + pixels[i + 2];
//
//			//std::cout << colorValue << std::endl;
//
//			if (colorValue < 0.4) {
//				screen[pos] = ' ';
//			}
//			else if (colorValue < 0.8) {
//				screen[pos] = ',';
//			}
//			else if (colorValue < 1.6) {
//				screen[pos] = ';';
//			}
//			else if (colorValue < 1.2) {
//				screen[pos] = 'i';
//			}
//			else if (colorValue < 2.6) {
//				screen[pos] = '0';
//			}
//			else{
//				screen[pos] = '8';
//			}
//
//
//		}
//		screen[300 * 120] = '\0';
//		WriteConsoleOutputCharacter(hConsole, screen, 300 * 120, { 0,0 }, &dwBytesWritten);
//	}
//}

void World::clearScreen() {
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void World::processInput() {

	cam.Inputs(window);
	if (GetKeyState('R') & 0x8000) {
		gravityEnabled = true;
	}
	if (GetKeyState('Q') & 0x8000) {
		glfwSetWindowShouldClose(window, true);

	}

	if (GetKeyState('E') & 0x8000 && !isDriving && !pushed) {
		isDriving = true;
		pushed = true;
		if (helicopters.size() != 0) {
			isFlying = true;
			helicopter = &helicopters[0];
			//float g = helicopters[0].getModdel()->getVelocity().x;
			//std::cout << g;
			drivable = helicopters[0].getModdel();
			drivable->setCurrentlyFlying(true);
		}
		else {

			drivable = models[2];
			isDriving = true;
			drivable->setCurrentlyDriving(true);

		}
		cam.setIsDriving(true);
	}
	else if ((!(GetKeyState('E') & 0x8000) && pushed)) {
		pushed = false;
	}
	else if (isFlying) {
		//std::cout << "ran";
		helicopter->processInput(deltaT);
		cam.setVehichleCamera(*drivable->getRawTransformation(), *drivable->getCm(), window);

	}
	else if (isDriving) {
		if (GetKeyState('E') & 0x8000 && !pushed) {
			isDriving = false;
			pushed = true;
			drivable->setCurrentlyDriving(false);
			cam.setIsDriving(false);
		}
		if (GetKeyState(VK_UP) & 0x8000) {
			drivable->accelerateForward(60.0, deltaT);
		}

		if (GetKeyState(VK_RIGHT) & 0x8000) {
			drivable->setWantedRotation(4.0);
		}
		else if (GetKeyState(VK_LEFT) & 0x8000) {
			drivable->setWantedRotation(-4.0);
		}
		else {
			drivable->setWantedRotation(0.0);
		}

		if (GetKeyState(VK_DOWN) & 0x8000) {
			drivable->accelerateForward(-80.0, deltaT);

		}

		if (GetKeyState('R') & 0x8000) {
			glm::vec3 v = glm::vec3(0);
			drivable->setVelocity(v);

		}
		cam.setVehichleCamera(*drivable->getRawTransformation(), *drivable->getCm(), window);
	}
	
}

void World::renderModels() {
	//glLineWidth(2);
	//glPolygonMode()
	for (int i = 0; i < models.size(); i++) {
		models[i]->Draw(cam);
	}

	for (int i = 0; i < floors.size(); i++) {
		floors[i].Draw(cam);
	}

	for (int i = 0; i < explosionModels.size(); i++) {
		if (explosionModels[i].getEnabled()) {
			explosionModels[i].draw();

		}
	}

}

void World::update() {
	for (int i = 0; i < models.size(); i++) {
		models[i]->update(deltaT);
	}
	for (int i = 0; i < helicopters.size(); i++) {
		helicopters[i].update(deltaT);
	}
	for (int i = 0; i < explosionModels.size(); i++) {
		explosionModels[i].update(deltaT);
	}
}












void World::comb(int N, int K, std::vector<int>& returning) {
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






void World::changeIsMovable(int pos, bool newValue) {
	models[pos]->changeIsMovable(newValue);
}



void World::generateAxis_Hitboxes(Hitbox& hitbox1, Hitbox& hitbox2, std::vector<glm::vec3>& returning) {
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




void World::detectCollisions() {
	std::vector<int> modelsToCheck;
	comb(models.size(), 2, modelsToCheck);
	for (int i = 0; i < modelsToCheck.size(); i += 2) {
		glm::vec3 normal;
		float intersectBy;
		IntersectionModel model;
		bool colliding = checkHitboxes(*models[modelsToCheck[i]], *models[modelsToCheck[i + 1]], model);
		if (colliding && models[modelsToCheck[i]]->getType() == "M") {
			explosions.push_back(modelsToCheck[i]);
		}
		else if (colliding && models[modelsToCheck[i + 1]]->getType() == "M") {
			explosions.push_back(modelsToCheck[i+1]);
		}
		else if (colliding) {
			currentCollisions.push_back(model);
		}
	}
}



void World::startRenderLoop() {

	while (!glfwWindowShouldClose(window))
	{
		deltaT = glfwGetTime() - time;
		time = glfwGetTime();

		processInput();

		clearScreen();



		cam.setMatrix(90.0f, 5.0f, 2000.0f);

		update();
		detectCollisions();
		dealWithCollisions();
		dealWithExplosions();
		eraseExplosions();
		applyDealWithImpulses();
		applyDrag(deltaT);

		renderModels();

		//screenToPixel();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void World::applyDrag(float dt) {
	for (int i = 0; i < models.size(); i++) {
		if (models[i]->isMovable()) {
			models[i]->scaleVelocity(pow(dragConstant, -dt));
		}
	}
}


bool World::checkHitboxes(Model& model1, Model& model2, IntersectionModel& given) {
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





bool World::checkHitboxesColliding(Hitbox& hitbox1, Hitbox& hitbox2, float& curintersect, glm::vec3& normalToIntersect) {
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

void World::dealWithExplosions() {
	for (int i = 0; i < explosions.size(); i++) {
		Model* rocket = models[explosions[i]];
		for (int k = 0; k < explosionModels.size(); k++) {
			if (!explosionModels[k].getEnabled()) {
				explosionModels[k].enable(*rocket->getCm());
				break;
			}
		}
		for (int k = 0; k < models.size(); k++) {
			if (models[k]->getType() != "M") {
				applyExplosionForce(rocket, models[k]);
			}
		}

		
	}
}

void World::eraseExplosions() {
	for (int i = 0; i < explosions.size(); i++) {
		delete models[explosions[i]];
		models.erase(models.begin() + explosions[i]);

		for (int k = i+1; k < explosions.size(); k++) {
			if (explosions[i] > explosions[k]) {
				// nothing 
			}
			else if (explosions[i] < explosions[k]) {
				explosions[k] = explosions[k] - 1;
			}
			else if (explosions[i] == explosions[k]) {
				explosions.erase(explosions.begin() + k);
			}
		}
	}
	explosions.clear();
}

void World::applyExplosionForce(Model* missile, Model* object) {
	if (object->isMovable() && object->getType() != "A") {
		glm::vec3 impulse = *object->getCm() - *missile->getCm();
		impulse = glm::normalize(impulse) / MyMath::getVectorMagnitudeSquared(impulse) * 1000000000.0f;
		Impulse imp;
		imp.direction = impulse;
		imp.position = *object->getCm();
		imp.centerCollision = *object->getCm();
		object->addImpulse(imp);
	}
	
}


void World::dealWithCollisions() {



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

void World::dealWithBothMovable(int i) {


	glm::vec3 moveBy2 = currentCollisions[i].solutionVec / 2.0f;
	glm::vec3 moveBy1 = moveBy2 * (-1.0f);


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);



	currentCollisions[i].model1->moveBy(moveBy1);
	currentCollisions[i].model2->moveBy(moveBy2);
}

void World::dealWithFirstMovable(int i) {
	glm::vec3 moveBy1 = currentCollisions[i].solutionVec * (-1.0f);


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);

	currentCollisions[i].model1->moveBy(moveBy1);
}

void World::dealWithSecondMovable(int i) {
	glm::vec3 moveBy2 = currentCollisions[i].solutionVec * 1.0f;


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	currentCollisions[i].model2->moveBy(moveBy2);
}

void World::detectPointFace(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
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

void World::correctNormalsToPointRightDirection(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
	for (int i = 0; i < given.size(); i++) {
		glm::vec3 cmToPos = given[i].position - *h1.getCm();
		if (glm::dot(cmToPos, given[i].normal) < 0) {
			given[i].normal = given[i].normal * (-1.0f);
		}
	}
}


glm::vec3 World::getClosestPointsOnLines(glm::vec3& a0, glm::vec3& b0, glm::vec3& a1, glm::vec3& b1, bool& worked) {
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

void World::detectEdgeEdge(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
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

void World::generateContacts(Model& model1, Model& model2, std::vector <Contact>& given) {
	for (int i = 0; i < model1.getHitboxesSize(); i++) {
		for (int k = 0; k < model2.getHitboxesSize(); k++) {


			detectPointFace(*model1.getHitbox(i), *model2.getHitbox(k), given);
			detectPointFace(*model2.getHitbox(k), *model1.getHitbox(i), given);
			//for (int i = 0; i < given.size(); i++) {
			//	std::cout << given[i].normal.x << " " << given[i].normal.y << " " << given[i].normal.z << "\n";
			//}

			//std::cout << given.size() << "\n";
			//for (int i = 0; i < given.size(); i++) {
			//	std::cout << "\nvec: \n";
			//	std::cout << given[i].position.x << " " << given[i].position.y << " " << given[i].position.z << "\n";
			//	std::cout << given[i].normal.x << " " << given[i].normal.y << " " << given[i].normal.z << "\n";
			//}

			detectEdgeEdge(*model1.getHitbox(i), *model2.getHitbox(k), given);
			correctNormalsToPointRightDirection(*model1.getHitbox(i), *model2.getHitbox(k), given);

			//std::cout << given.size() << "\n";
			//for (int i = 0; i < given.size(); i++) {
			//	std::cout << "\nvec: \n";
			//	std::cout << given[i].position.x << " " << given[i].position.y << " " << given[i].position.z << "\n";
			//	std::cout << given[i].normal.x << " " << given[i].normal.y << " " << given[i].normal.z << "\n";
			//}

		}
	}


}


void World::addImpulses(Model& model1, Model& model2, std::vector<Contact>& vec) {

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



void World::applyDealWithImpulses() {
	for (int i = 0; i < models.size(); i++) {
		models[i]->dealWithImpulses();
		models[i]->dealWithFrictionForce();
	}
}

Model* World::getModel(int i) {
	return models[i];
}

glm::vec3 World::calculateTVec(Model& m1, Model& m2, glm::vec3& pos, glm::vec3& normal) {
	glm::vec3 vr = m1.velocityAtPoint(pos) - m2.velocityAtPoint(pos);

	if (glm::dot(vr, normal) != 0) {
		glm::vec3 case1 = vr - normal * glm::dot(vr, normal);
		return case1;
	}
	//}else if()
}

void World::dealWithImpulse(Model& m1, Model& m2) {
	m1.dealWithImpulses();
	m2.dealWithImpulses();
}

glm::vec3* World::getGravity() {
	return &gravityAcceleration;
}

int World::getModelsSize() {
	return models.size();
}

void World::setTransformation(int i, glm::mat4& g) {
	models[i]->setTransformation(g);
}
//glm::vec3 World::getRelativeVelocities(Model& m1, Model& m2, glm::vec3 & atPoint, glm::vec3 & normal) {
//	return 
//}

void World::setupExplosions() {
	for (int i = 0; i < 30; i++) {
		Model* t = new Model("Explosion.object", 30, glm::vec3(0,90,0), 3);
		explosionModels.push_back(Explosion(t, &cam));
	}
}







//glfwSetWindowShouldClose(window, true);
