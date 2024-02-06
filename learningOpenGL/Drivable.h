#pragma once
#include "Model.h"

class Drivable : public Model
{
public:
	virtual void handleInputs(float dt) = 0;
	virtual void cameraControls(Camera & cam) = 0;
};

