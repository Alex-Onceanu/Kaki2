

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
	try
	{
		currentWorld = std::make_unique<MainWorld>(running);
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << e << std::endl;
	}
}

Game::~Game()
{
	
}

void Game::MainLoop()
{
	/*int preFrame, postFrame, duree;
	const double targetDuree = 50. / 3;*/
	while (running)
	{
		//preFrame = Renderer::GetTicks();
		Frame();
		/*postFrame = Renderer::GetTicks();
		duree = postFrame - preFrame;*/

		////On a ainsi la durée d'une frame
		////Or pour du 60FPS il faut que chaque frame dure 1000/60 = 50/3 = 16.6666667 ms
		//if (duree < targetDuree)
		//{
		//	//Donc si la frame a été trop rapide on attend quelques ms histoire de bien être à 16.666 ms par frame.
		//	//Renderer::Wait(targetDuree - duree);
		//}
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