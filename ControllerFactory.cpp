#include "ControllerFactory.h"

#include "EntityController.h"
#include "AnimationController.h"
#include "PlayerMovementController.h"
#include "PositionController.h"
#include "StaticDrawController.h"
#include "CollisionController.h"
#include "ObstacleAnimationController.h"


#include <cassert>
#include <memory>

ControllerFactory::ControllerFactory()
{
	std::vector<std::string> strings
	{
		"Animation",
		"PlayerMovement",
		"Position",
		"StaticDraw",
		"Collision",
		"ObstacleAnimation",
	};

	std::vector<EntityControllerCreator*> creators
	{
		new AnimationControllerCreator(),
		new PlayerMovementControllerCreator(),
		new PositionControllerCreator(),
		new StaticDrawControllerCreator(),
		new CollisionControllerCreator(),
		new ObstacleAnimationControllerCreator(),
	};

	assert(strings.size() == creators.size());

	for (int i = 0; i < strings.size(); i++)
	{
		strToCreator.emplace(strings[i], creators[i]);
	}
}

ControllerFactory::~ControllerFactory()
{
	for (auto i : strToCreator)
	{
		delete(i.second);
	}
}

std::unique_ptr<EntityController> ControllerFactory::Create(Entity* owner, std::string ini)
{
	return std::move((*strToCreator[ini])(owner));
}