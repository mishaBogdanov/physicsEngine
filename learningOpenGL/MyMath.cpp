#include "MyMath.h"
#include <iostream>


void MyMath::vectorCross(glm::vec3& vec1, glm::vec3& vec2, glm::vec3& returning) {
	returning.x = vec1.y * vec2.z - vec1.z * vec2.y;
	returning.y = vec1.z * vec2.x - vec1.x * vec2.z;
	returning.z = vec1.x * vec2.y - vec1.y * vec2.x;
}

void MyMath::projection(glm::vec3& ontoProjection, glm::vec3& projected, glm::vec3& returning) {
	returning = ontoProjection * (ontoProjection.x * projected.x + ontoProjection.y * projected.y + ontoProjection.z * projected.z) /
		(ontoProjection.x * ontoProjection.x + ontoProjection.y * ontoProjection.y + ontoProjection.z * ontoProjection.z);
}

float MyMath::getVectorMagnitudeSquared(glm::vec3 & given) {
	return given.x * given.x + given.y * given.y + given.z * given.z;
}

float MyMath::getVectorMagnitude(glm::vec3& given) {
	return sqrt(given.x * given.x + given.y * given.y + given.z * given.z);
}


float MyMath::getSumParts(glm::vec3& given) {
	return given.x + given.y + given.z;
}

float MyMath::getVecMultiple(glm::vec3& first, glm::vec3& second_the_one_that_gets_divided) {
	if (first.x != 0) {
		return second_the_one_that_gets_divided.x/first.x;
	}
	else if (first.y != 0) {
		return second_the_one_that_gets_divided.y/first.y;
	}
	else if (first.z != 0) {
		return second_the_one_that_gets_divided.z/first.z;
	}
	else {
		std::cout << ":(";
	}
}