#pragma once

#include "World.h"
#include "TextureManager.h"
#include "InputEventsEnum.h"
#include "EventEnum.h"

#include <unordered_map>

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
	void PostEventFromInput();
	void InitKeyToEvent();

protected:
	std::unique_ptr<TextureManager> tm;
	//std::unique_ptr<std::vector<std::pair<KeyInput, EventEnum>>> keyToEvent;
	std::unique_ptr<std::unordered_map<KeyInput, EventEnum>> keyToEvent;
};

