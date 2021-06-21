#pragma once

#include <vector>
#include <memory>

#include "Entity.h"
#include "TextureManager.h"

class World
{
public:

	World()
	{
		textureManager = std::make_unique<TextureManager>();
	}

	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	std::unique_ptr<TextureManager> textureManager;
};