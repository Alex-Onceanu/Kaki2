#pragma once

class Entity;

#include "Position.h"

#include <memory>
#include <map>
#include <string>

class EntityController
{
public:
	EntityController(Entity* o)
		: owner(o) {};

	virtual void ProcessInput() {}
	virtual void Update() {};
	virtual void Draw(const Position &cameraPos) {};

	virtual void LoadInitialData(std::map<std::string, std::string>& ini) {};
	
protected:
	Entity* owner;
};

class EntityControllerCreator
{
public:
	virtual std::unique_ptr<EntityController> operator()(Entity* owner) = 0;
};