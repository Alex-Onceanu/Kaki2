#pragma once
#include "EntityController.h"
#include "Texture.h"
#include "Position.h"

#include <vector>
#include <memory>
#include <string>

class ObstacleAnimationController :
    public EntityController
{
public:
	ObstacleAnimationController(Entity* const o);
	void LoadInitialData(std::map<std::string, std::string>& ini) override;

	void SetPath(const std::string path);

	void Update() override;
	void Draw(const Position& cameraPos) override;
private:
	std::unique_ptr<std::vector<std::shared_ptr<Texture>>> frames;
	
	const Position* owner_pos;
	int* owner_w;
	int* owner_h;

	int frameCount = 0;
	int animCount = 0;
};

class ObstacleAnimationControllerCreator : public EntityControllerCreator
{
public:
	std::unique_ptr<EntityController> operator()(Entity* owner) override;
};
