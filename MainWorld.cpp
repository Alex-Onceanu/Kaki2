#include "MainWorld.h"
#include "MC.h"
#include "TextureManager.h"
#include "Input.h"
#include "InputEventsEnum.h"
#include "EventSystem.h"
#include "EventEnum.h"
#include "Camera.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

MainWorld::MainWorld(bool& running)
	:World()
{
	tm = make_unique<TextureManager>();
	InitKeyToEvent();

	CreateWorld();
}

void MainWorld::InitKeyToEvent()
{
	std::vector<InputEventEnum> keys{
		InputEventEnum::QUIT,

		InputEventEnum::W_down,
		InputEventEnum::S_down,
		InputEventEnum::D_down,
		InputEventEnum::A_down,
	
		InputEventEnum::W_up,
		InputEventEnum::S_up,
		InputEventEnum::D_up,
		InputEventEnum::A_up,
	};
	std::vector<EventEnum> events{
		EventEnum::QUIT_GAME,

		EventEnum::MOVE_UP,
		EventEnum::MOVE_DOWN,
		EventEnum::MOVE_RIGHT,
		EventEnum::MOVE_LEFT,
	
		EventEnum::STOP_UP,
		EventEnum::STOP_DOWN,
		EventEnum::STOP_RIGHT,
		EventEnum::STOP_LEFT,
	};

	assert(keys.size() == events.size());

	keyToEvent = std::make_unique<std::map<InputEventEnum, EventEnum>>();

	for (int i = 0; i < keys.size(); i++)
	{
		keyToEvent->insert({ keys[i],events[i] });
	}
}

void MainWorld::CreateWorld()
{
	Position p = { 200 - RES_X / 2,200 - RES_Y / 2 };
	camera = std::make_unique<Camera>(p);
	entities.push_back(std::make_unique<MC>(tm.get()));
}

MainWorld::~MainWorld()
{

}

void MainWorld::PostEventFromInput()
{
	//On lance les evenements attribues aux touches

	InputEventEnum nextEvent;
	while (Input::GetNextInputEvent(&nextEvent))
	{
		auto key = keyToEvent->find(nextEvent);
		if (key != keyToEvent->end())
		{
			auto e = Event(key->second);
			EventSystem::Post(e);
		}
	}
}

void MainWorld::ProcessInput()
{
	PostEventFromInput();

	for (auto&& e : entities)
	{
		e->ProcessInput();
	}
}

void MainWorld::Update()
{
#if 0
	auto camp = entities[0]->GetPosition();
	if (camp.x > -500 and camp.x < 3000
		and camp.y > -300 and camp.y < 2000)
	{
	}
#endif
	camera->Update();
	
	for (auto&& e : entities)
	{
		e->Update();
	}
}

void MainWorld::Draw()
{
	Renderer::Clear(50, 200, 80);

	Position camPos = camera->GetPos();

	for (auto&& e : entities)
	{
		e->Draw(camPos);
	}
}