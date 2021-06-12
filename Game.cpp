

#include <memory>

#include "Game.h"
#include "World.h"
#include "MainWorld.h"
#include "EventsEnum.h"
#include "Input.h"

Game::Game()
	:running(true)
{
	currentWorld = std::make_unique<MainWorld>(running);
}

Game::~Game()
{
	
}

void Game::MainLoop()
{
	while (running)
	{
		//handle quit event
		Frame();
	}
}

void Game::Frame()
{
	currentWorld->ProcessInput();
	currentWorld->Update();
	currentWorld->Draw();
}