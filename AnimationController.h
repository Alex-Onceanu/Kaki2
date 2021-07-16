#pragma once
#include "EntityController.h"
#include "Position.h"
#include "TextureManager.h"

#include <string>
#include <memory>

class AnimationController : public EntityController
{
public:
	AnimationController(Entity* const o, TextureManager* tm_, const std::string folderPath_);
	
	void Update() override;
	void Draw(const Position &cameraPos) override;

private:
	void LoadAllImages();

private:

	enum animationsIndex
	{
		INDEX_UP, INDEX_DOWN, INDEX_LEFT
	};

	std::unique_ptr<std::vector<std::vector<std::shared_ptr<Texture>>>> allAnimations;
	std::unique_ptr<std::vector<std::shared_ptr<Texture>>> currentAnimation;

	TextureManager* tm;
	std::string folderPath;

	Position oldPosition;

	const Position* owner_pos;
	int* owner_w;
	int* owner_h;

	bool shouldMirrorBlit = false;
	int frameCount = 0;
	int animCount = 0;
};