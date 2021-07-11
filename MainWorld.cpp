#include "MainWorld.h"
#include "MC.h"
#include "TextureManager.h"
#include "Input.h"
#include "InputEventsEnum.h"
#include "EventSystem.h"
#include "EventEnum.h"
#include "Camera.h"
#include "Resolution.h"
#include "Utility.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Tools/Map.h"

#define PEEK()\
		Rect groundRect = { 0, 0, scene->size_x + RES_X, scene->size_y + RES_Y };\
		this->ground = std::make_unique<Texture>(*Renderer::GetRenderer(), &gnd);\
		Renderer::FullBlit(ground.get(), groundRect);\
		Renderer::Flip();

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

	LoadScene(1);
	LoadGround();
}

void MainWorld::LoadScene(int nb_)
{
	scene = std::make_unique<Map>();
	std::ostringstream path;
	path << "./Content/Maps/" << nb_ << ".bin";

	try
	{
		MapFile fichier(path.str().c_str(), std::ios::binary);

		const char* password{ "KAKI2" };
		char* pass = (char *)alloca(strlen(password) + 1);
		std::memset(pass, '\0', strlen(password) + 1);
		fichier.read(pass, strlen(password));

		int key;
		fichier.read((char *)&key, sizeof(key));

		if (strstr(pass, password) && key == 2)
		{
			fichier >> *scene;
			RemoveZerosStr(scene->tiles);
		}
	}
	catch (std::exception e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << e.what() << std::endl;
	}
}

//void MainWorld::LoadGround()
//{
//	auto sol = textureManager->GetSurface("./Assets/Sol/1.png");
//	int solW;
//	int solH;
//	sol->GetSize(solW, solH);
//
//	const int mapX = scene->size_x + RES_X;
//	const int mapY = scene->size_y + RES_Y;
//	auto gnd = Surface(mapX, mapY);
//	
//	for (int x = 0; x < mapX; x += solW)
//	{
//		for (int y = 0; y < mapY; y += solH)
//		{
//			Rect dst = { x,y,solW,solH };
//			Renderer::CopySurface(sol.get(), nullptr, &gnd, &dst);
//		}
//	}
//	ground = std::make_unique<Texture>(*Renderer::GetRenderer(), &gnd);
//}
//
//void MainWorld::LoadGround()
//{
//	std::string str;
//	while (str != ";")
//	{
//		for (char c : str)
//		{
//			std::string path = "./Assets/Sol/" + c + (std::string)".png";
//			auto bloc = textureManager->GetSurface(path);
//		}
//	}
//}

//void MainWorld::LoadGround()
//{
//	// Tant qu'on depasse pas la surface et qu'il reste des tiles a placer on lit le prochain bloc et on le place
//
//	const int map_w = scene->size_x;
//	const int map_h = scene->size_y;
//	auto gnd = Surface(map_w, map_h);
//	Rect dst;
//	
//	for (int y = 0; y < map_h; y += scene->tile_h)
//	{
//		for (int x{ 0 }, c{ 0 }; (x < map_w) and (c < scene->tiles.length()); x += scene->tile_w, c++)
//		{
//			std::string path = std::string("./Assets/Sol/") + char(scene->tiles[c]) + std::string(".png");
//			auto bloc = textureManager->GetSurface(path);
//
//			dst = { x,y,scene->tile_w,scene->tile_h };
//			Renderer::CopySurface(bloc.get(), nullptr, &gnd, &dst);
//		}
//	}
//	this->ground = std::make_unique<Texture>(*Renderer::GetRenderer(), &gnd);
//}

void MainWorld::LoadGround()
{
	// Tant qu'on depasse pas la surface et qu'il reste des tiles a placer on lit le prochain bloc et on le place

	const int map_w = scene->size_x;
	const int map_h = scene->size_y;
	auto gnd = Surface(map_w, map_h);
	Rect dst;
	std::vector<std::shared_ptr<Surface>> blocs;

	bool go = true;
	for (int i = 1; go; i++)
	{
		try
		{
			std::ostringstream path;
			path << "./Assets/Sol/" << i << ".png";
			blocs.push_back(textureManager->GetSurface(path.str()));
		}
		catch (...)
		{
			go = false;
		}
	}

	int c{ 0 };
	for (int y = 0; y < map_h; y += scene->tile_h)
	{
		for (int x{ 0 }; (x < map_w) and (c < scene->tiles.length()); x += scene->tile_w)
		{
			dst = { x,y,scene->tile_w,scene->tile_h };
			Renderer::CopySurfaceScaled(blocs[scene->tiles[c++] - 49].get(), nullptr, &gnd, &dst);
		}
	}
	this->ground = std::make_unique<Texture>(*Renderer::GetRenderer(), &gnd);
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
	camera->UpdatePosition(scene->size_x, scene->size_y);
}

void MainWorld::Draw()
{
	Renderer::Clear(50, 200, 80);


	Position camPos = camera->GetPos();

	Rect groundRect = { -camPos.x, -camPos.y, scene->size_x + RES_X, scene->size_y + RES_Y };
	Renderer::FullBlit(ground.get(), groundRect);

	for (auto&& e : entities)
	{
		e->Draw(camPos);
	}
}