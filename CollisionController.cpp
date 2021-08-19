#include "CollisionController.h"
#include "EventEnum.h"
#include "Entity.h"
#include "Renderer.h"

#include <iostream>

CollisionController::CollisionController(Entity* o, bool movible_)
	: EntityController(o)
	, Listener()
{
	owner->SetMovible(movible_);
	owner->SetSolid(true);
}

bool CollisionController::CanProcess(Event* e)
{
	return((e->type == EventEnum::COLLISION)
		and (static_cast<Entity*>(e->args) == owner));
}

void CollisionController::OnEvent(Event* e)
{
	if (owner->IsMovible())
	{
		owner->SetPosition(oldPosition);
	}
}

//void CollisionController::Update()
//{
//	
//}

void CollisionController::Draw(const Position& cameraPos)
{
	oldPosition = owner->GetPosition();

	if (shouldDrawCollisionBox)
	{
		Renderer::DrawRect(*owner->GetRectPtr(), 255, 0, 0);
	}
}