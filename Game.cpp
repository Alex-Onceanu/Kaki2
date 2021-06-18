

#include <memory>

#include "Game.h"
#include "World.h"
#include "MainWorld.h"
#include "EventEnum.h"
#include "Input.h"
#include "Renderer.h"
#include "EventSystem.h"

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
		running = !Input::CheckEvent(GeneralInput::QUIT);
		Frame();
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