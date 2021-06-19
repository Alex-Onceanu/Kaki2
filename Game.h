#pragma once

#include "World.h"
#include "Event.h"

class Game
	:public Listener
{
public:
	Game();
	~Game();

	void MainLoop();
	void OnEvent(Event* e) override;
	virtual bool CanProcess(Event* e) override;

private:
	void Frame();

private:
	std::unique_ptr<World> currentWorld;

	bool running;
};

