#pragma once

#include <memory>

#include "Texture.h"
#include "EntityController.h"

class StaticDrawController : public EntityController
{
public:
	StaticDrawController(Entity* o);
	void LoadInitialData(std::map<std::string, std::string>& ini) override;
	
	void SetPath(const std::string& path);

	void Draw(const Position &cameraPos) override;

private:
	std::shared_ptr<Texture> image = nullptr;

	const Position* owner_pos;
	int* owner_w;
	int* owner_h;
};

class StaticDrawControllerCreator : public EntityControllerCreator
{
public:
	std::unique_ptr<EntityController> operator()(Entity* owner) override;
};
