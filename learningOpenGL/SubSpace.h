#pragma once
#include "Model.h"
#include <vector>

class SubSpace
{
private:
	std::vector<Model*> models;
	void detectCollisions();
	void dealWithCollisions();
public:
	void addModel(Model* givenModel);
	void collisions();
};

