#pragma once
#include "Position.h"

class Entity
{
public:
	virtual ~Entity() {};

	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Draw(Position cameraPos) = 0;
};

