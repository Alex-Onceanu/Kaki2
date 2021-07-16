#pragma once
#include "Position.h"
class Entity;



class EntityController
{
public:
	EntityController(Entity* o)
		: owner(o) {};

	virtual void ProcessInput() {}
	virtual void Update() {};
	virtual void Draw(const Position &cameraPos) {};
	
protected:
	Entity* owner;
};
