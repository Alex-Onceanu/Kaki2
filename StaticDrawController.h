#pragma once

#include <string>

#include "EntityController.h"
#include "TextureManager.h"


class StaticDrawController : public EntityController
{
public:
	StaticDrawController(Entity* o, TextureManager* tm_, const std::string path);

	void Draw(const Position &cameraPos) override;

private:
	TextureManager* tm;

	std::shared_ptr<Texture> image;

	const Position* owner_pos;
	int* owner_w;
	int* owner_h;
};

