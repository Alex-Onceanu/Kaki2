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

MainWorld::MainWorld(bool& running)
	:World()
{
	tm = make_unique<TextureManager>();
	InitKeyToEvent();

	CreateWorld();
}

void MainWorld::InitKeyToEvent()
{
	//On attribue a chaque touche un evenement a lancer

	std::vector<KeyEvent> keys{
		K_w,
		K_s,
		K_d,
		K_a };
	std::vector<EventEnum> events{
		EventEnum::MOVE_UP,
		EventEnum::MOVE_DOWN,
		EventEnum::MOVE_RIGHT,
		EventEnum::MOVE_LEFT };

	assert(keys.size() == events.size());

	keyToEvent = make_unique<std::vector<std::pair<KeyEvent, EventEnum>>>();
	
	for (int i = 0; i < keys.size(); i++)
	{
		keyToEvent->push_back(std::make_pair(keys[i], events[i]));
	}

	std::sort(keyToEvent->begin(), keyToEvent->end(),
		[](std::pair<KeyEvent, EventEnum> a, std::pair<KeyEvent, EventEnum> b)
		{ return static_cast<int>(a.second) < static_cast<int>(b.second); }
	);
}

void MainWorld::CreateWorld()
{
	entities.push_back(std::make_unique<MC>(tm.get()));
}

MainWorld::~MainWorld()
{

}

void MainWorld::LaunchEventFromInput()
{
	//On lance les evenements attribues aux touches


}

void MainWorld::ProcessInput()
{
	

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