#pragma once

#include <string>

#include "EntityController.h"

class Entity;

class ControllerFactory
{
public:
	ControllerFactory();
	~ControllerFactory();
	std::unique_ptr<EntityController> Create(Entity* owner, std::string ini);

private:
	std::map<std::string, EntityControllerCreator*> strToCreator;
};