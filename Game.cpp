

#include <memory>

#include "Game.h"
#include "World.h"
#include "MainWorld.h"
#include "EventEnum.h"
#include "Input.h"
#include "Renderer.h"
#include "EventSystem.h"
#include "Event.h"

Game::Game()
	:Listener(),
	running(true)
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
		Frame();
	}
}

bool Game::CanProcess(Event* e)
{
	return e->type == EventEnum::QUIT_GAME;
}

void Game::OnEvent(Event* e)
{
	if (e->type == EventEnum::QUIT_GAME)
	{
		running = false;
	}
}


void Game::Frame()
{
	currentWorld->ProcessInput();
	EventSystem::Update();
	currentWorld->Update();
	currentWorld->Draw();
	Renderer::Flip();
}