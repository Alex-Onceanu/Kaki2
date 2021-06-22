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
	auto mc = std::make_unique<MC>(tm.get());
	const int* w;
	const int* h;
	mc->GetSizePtr(&w, &h);
	camera = std::make_unique<Camera>(mc->GetPositionPtr(), w, h);
	entities.push_back(std::move(mc));
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
	for (auto&& e : entities)
	{
		e->Update();
	}

	//La camera suit toujours le joueur, mais doit s'arreter
	//Aux limites de la map : 0,0 et camLimit
	constexpr int camLimitX = 1500;
	constexpr int camLimitY = 2000;
	camera->UpdatePosition(camLimitX, camLimitY);
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