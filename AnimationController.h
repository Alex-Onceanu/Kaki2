#pragma once

#include "Texture.h"
#include "EntityController.h"
#include "Position.h"
#include "TextureManager.h"
#include "Event.h"

#include <vector>
#include <memory>
#include <string>

class AnimationController
	: public EntityController
	, public Listener
{
public:
	AnimationController(Entity* const o);
	void LoadInitialData(std::map<std::string, std::string>& ini) override;

	void SetPath(const std::string path);

	void Update() override;
	void Draw(const Position &cameraPos) override;

	bool CanProcess(Event* e) override;
	void OnEvent(Event* e) override;

private:
	void LoadAllImages();

private:

	enum animationsIndex
	{
		INDEX_UP, INDEX_DOWN, INDEX_LEFT
	};

	std::unique_ptr<std::vector<std::vector<std::shared_ptr<Texture>>>> allAnimations;
	std::unique_ptr<std::vector<std::shared_ptr<Texture>>> currentAnimation;
	
	std::string folderPath;

	Position oldPosition;

	const Position* owner_pos;
	int* owner_w;
	int* owner_h;

	bool shouldMirrorBlit = false;
	int frameCount = 0;
	int animCount = 0;
};

class AnimationControllerCreator : public EntityControllerCreator
{
public:
	std::unique_ptr<EntityController> operator()(Entity* owner) override;
};
