#include "CollisionController.h"
#include "EventEnum.h"
#include "Entity.h"
#include "Renderer.h"
#include <algorithm>
#include <iostream>
#include <sstream>

CollisionController::CollisionController(Entity* o)
	: EntityController(o)
	, Listener()
{
	owner->SetSolid(true);
}

void CollisionController::LoadInitialData(std::map<std::string, std::string>& ini)
{
	auto str = ini["movible"];
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream ss(str);
	bool b;
	ss >> std::boolalpha >> b;
	owner->SetMovible(b);
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

void CollisionController::Draw(const Position& cameraPos)
{
	oldPosition = owner->GetPosition();

	if (shouldDrawCollisionBox)
	{
		Renderer::DrawRect(*owner->GetRectPtr(), 255, 0, 0);
	}
}

std::unique_ptr<EntityController> CollisionControllerCreator::operator()(Entity* owner)
{
	return std::move(std::make_unique<CollisionController>(owner));
}