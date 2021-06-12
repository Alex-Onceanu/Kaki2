#include "MainWorld.h"

#include "TextureManager.h"

MainWorld::MainWorld(bool& running)
	:World()
{
	CreateWorld();
}

void MainWorld::CreateWorld()
{
	
}

MainWorld::~MainWorld()
{

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