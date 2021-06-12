

#include <memory>

#include "Game.h"
#include "World.h"
#include "MainWorld.h"
#include "EventsEnum.h"
#include "Input.h"
#include "Renderer.h"

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
		running = !Input::CheckEvent(QUIT);
		Frame();
	}
}

void Game::Frame()
{
	currentWorld->ProcessInput();
	currentWorld->Update();
	currentWorld->Draw();
	Renderer::Flip();
}