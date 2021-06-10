#pragma once

#include "World.h"

class MainWorld : public World
{
public:
	MainWorld(bool& running);
	~MainWorld();

	void CreateWorld();

	void ProcessInput() override;
	void Update() override;
	void Draw() override;
};

