#pragma once

#include <vector>
#include <memory>

#include "Entity.h"
#include "TextureManager.h"

class World
{
public:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};