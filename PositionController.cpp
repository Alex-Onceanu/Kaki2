#include "PositionController.h"
#include "Entity.h"

PositionController::PositionController(Entity * o, Position initialPos)
	: EntityController(o)
{
	ownerPositionPtr = o->GetPositionPtr();
	ownerRectPtr = o->GetRectPtr();

	*ownerPositionPtr = initialPos;
}

void PositionController::Draw(const Position& cameraPos)
{
	*ownerRectPtr = *ownerPositionPtr - cameraPos;
};