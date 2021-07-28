#include "PositionController.h"
#include "Entity.h"

PositionController::PositionController(Entity * o)
	: EntityController(o)
{
	ownerPositionPtr = o->GetPositionPtr();
	ownerRectPtr = o->GetRectPtr();
}

void PositionController::Draw(const Position& cameraPos)
{
	*ownerRectPtr = *ownerPositionPtr - cameraPos;
};

std::unique_ptr<EntityController> PositionControllerCreator::operator()(Entity* owner)
{
	return std::move(std::make_unique<PositionController>(owner));
}