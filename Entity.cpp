#include "Entity.h"
#include "ControllerFactory.h"
#include "IniReader.h"
#include "Position.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

Entity::Entity()
{
	position = { 0,0 };
	rect = { 0,0,0,0 };
};

void Entity::SetControllers(std::vector<std::unique_ptr<EntityController>>& args)
{
	controllers = std::move(args);
}

void Entity::LoadInitialData(ControllerFactory* cf, InitialData& ini)
{
	for (auto it : ini.data)
	{
		auto ctrl = cf->Create(this, it.first);
		ctrl->LoadInitialData(it.second);
		controllers.push_back(std::move(ctrl));
	}
}

void Entity::ProcessInput()
{
	for (auto&& c : controllers)
	{
		c->ProcessInput();
	}
}

void Entity::Update()
{
	for (auto&& c : controllers)
	{
		c->Update();
	}
}

void Entity::Draw(Position cameraPos)
{
	for (auto&& c : controllers)
	{
		c->Draw(cameraPos);
	}
}