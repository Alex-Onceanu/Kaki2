#include "MainWorld.h"
#include "MC.h"
#include "TextureManager.h"
#include "Input.h"
#include "InputEventsEnum.h"
#include "EventSystem.h"
#include "EventEnum.h"

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

#if 0

void MainWorld::InitKeyToEvent()
{
	//On attribue a chaque touche un evenement a lancer

	std::vector<KeyInput> keys{
		KeyInput::W,
		KeyInput::S,
		KeyInput::D,
		KeyInput::A };
	std::vector<EventEnum> events{
		EventEnum::MOVE_UP,
		EventEnum::MOVE_DOWN,
		EventEnum::MOVE_RIGHT,
		EventEnum::MOVE_LEFT };

	assert(keys.size() == events.size());

	keyToEvent = make_unique<std::vector<std::pair<KeyInput, EventEnum>>>();
	
	for (int i = 0; i < keys.size(); i++)
	{
		keyToEvent->push_back(std::make_pair(keys[i], events[i]));
	}

	std::sort(keyToEvent->begin(), keyToEvent->end(),
		[](std::pair<KeyInput, EventEnum> a, std::pair<KeyInput, EventEnum> b)
		{ return static_cast<int>(a.second) < static_cast<int>(b.second); }
	);
}

#endif

void MainWorld::InitKeyToEvent()
{
	std::vector<InputEventEnum> keys{
		InputEventEnum::QUIT,
		InputEventEnum::W,
		InputEventEnum::S,
		InputEventEnum::D,
		InputEventEnum::A };
	std::vector<EventEnum> events{
		EventEnum::QUIT_GAME,
		EventEnum::MOVE_UP,
		EventEnum::MOVE_DOWN,
		EventEnum::MOVE_RIGHT,
		EventEnum::MOVE_LEFT };

	assert(keys.size() == events.size());

	keyToEvent = std::make_unique<std::unordered_map<InputEventEnum, EventEnum>>();

	for (int i = 0; i < keys.size(); i++)
	{
		keyToEvent->insert({ keys[i],events[i] });
	}
}

void MainWorld::CreateWorld()
{
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
		std::cout << static_cast<int>(nextEvent) << std::endl;
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
}

void MainWorld::Draw()
{
	Renderer::Clear(50, 200, 80);

	for (auto&& e : entities)
	{
		e->Draw();
	}
}