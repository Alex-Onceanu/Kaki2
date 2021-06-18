#pragma once

#include "World.h"
#include "TextureManager.h"
#include "InputEventsEnum.h"
#include "EventEnum.h"

class MainWorld : public World
{
public:
	MainWorld(bool& running);
	~MainWorld();

	void ProcessInput() override;
	void Update() override;
	void Draw() override;

private:
	void CreateWorld();
	void LaunchEventFromInput();
	void InitKeyToEvent();

protected:
	std::unique_ptr<TextureManager> tm;
	std::unique_ptr<std::vector<std::pair<KeyEvent, EventEnum>>> keyToEvent;
};

