#include "MainWorld.h"
#include "MC.h"
#include "TextureManager.h"
#include <memory>

MainWorld::MainWorld(bool& running)
	:World()
{
	tm = make_unique<TextureManager>();
	CreateWorld();
}

void MainWorld::CreateWorld()
{
	entities.push_back(std::make_unique<MC>(tm.get()));
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