#pragma once

#include "World.h"

class Game
{
public:
	Game();
	~Game();

	void MainLoop();

private:
	void Frame();

private:
	std::unique_ptr<World> currentWorld;

	bool running;
};

