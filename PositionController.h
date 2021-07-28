#pragma once

#include "EntityController.h"
#include "Rect.h"
#include "Position.h"

#include <memory>

class PositionController : public EntityController
{
public:
	PositionController(Entity* o);

	void SetInitialPos(Position initialPos) { *ownerPositionPtr = initialPos; }

	void Draw(const Position& cameraPos) override;

private:
	Position* ownerPositionPtr;
	Rect* ownerRectPtr;
};

class PositionControllerCreator : public EntityControllerCreator
{
public:
	std::unique_ptr<EntityController> operator()(Entity* owner) override;
};
