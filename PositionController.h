#pragma once

#include "EntityController.h"
#include "Position.h"
#include "Rect.h"


class PositionController : public EntityController
{
public:
	PositionController(Entity* o, Position initialPos);

	void Draw(const Position& cameraPos) override;

private:
	Position* ownerPositionPtr;
	Rect* ownerRectPtr;
};

