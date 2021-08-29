#include "MainWorld.h"
#include "TextureManager.h"
#include "Input.h"
#include "InputEventsEnum.h"
#include "EventSystem.h"
#include "EventEnum.h"
#include "Camera.h"
#include "Resolution.h"
#include "Utility.h"

#include "ControllerFactory.h"
#include "EntityController.h"
#include "AnimationController.h"
#include "PositionController.h"
#include "StaticDrawController.h"
#include "PlayerMovementController.h"
#include "CollisionController.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Tools/Map.h"

MainWorld::MainWorld(bool& running)
	:World()
{
	cf = std::make_unique<ControllerFactory>();
	iniReader = std::make_unique<IniReader>();
	InitKeyToEvent();

	CreateWorld();
}

void MainWorld::InitKeyToEvent()
{
	std::vector<InputEventEnum> keys{
		InputEventEnum::QUIT,
		InputEventEnum::Q_down,
		InputEventEnum::ESC_down,

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
		EventEnum::QUIT_GAME,
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
	std::ifstream mc_ini("./Content/Entities/mc.ini");
	if (not mc_ini.is_open()) throw("Cannot open mc.ini");

	std::unique_ptr<Entity> mc = std::make_unique<Entity>();
	iniReader->Read(cf.get(), mc_ini, *mc);
	const int* w;
	const int* h;
	mc->GetSizePtr(&w, &h);
	camera = std::make_unique<Camera>(mc->GetPositionPtr(), w, h);
	entities.push_back(std::move(mc));

	LoadScene(1);
	LoadGround();

	std::srand(static_cast<int>(time(NULL)));
	for (int i = 0; i < 10; i++)
	{
		std::ifstream tree_ini("./Content/Entities/tree.ini");
		if (not tree_ini.is_open()) throw("Cannot open tree.ini");

		Position p{ std::rand() % (scene->size_x + RES_X),rand() % (scene->size_y + RES_Y) };
		std::unique_ptr<Entity> e = std::make_unique<Entity>();
		iniReader->Read(cf.get(), tree_ini, *e);
		e->SetPosition(p);
		entities.push_back(std::move(e));
	}
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

void MainWorld::LoadGround()
{
	// Tant qu'on depasse pas la surface et qu'il reste des tiles a placer on lit le prochain bloc et on le place

	const int map_w = scene->size_x;
	const int map_h = scene->size_y;
	auto gnd = Surface(map_w, map_h);
	Rect dst;
	std::vector<std::shared_ptr<Surface>> blocs;

	
	for (int i = 1; i < 10; i++)
	{
		std::ostringstream path;
		path << "./Assets/Sol/" << i << ".png";
		blocs.push_back(TextureManager::GetSurface(path.str()));
	}
	for (char i = 'a'; true; i++)
	{
		try
		{
			std::ostringstream path;
			path << "./Assets/Sol/" << i << ".png";
			blocs.push_back(TextureManager::GetSurface(path.str()));
		}
		catch (...)
		{
			break;
		}
	}

	int c{ 0 };
	for (int y = 0; y < map_h; y += scene->tile_h)
	{
		for (int x{ 0 }; (x < map_w) and (c < scene->tiles.length()); x += scene->tile_w)
		{
			dst = { x,y,scene->tile_w,scene->tile_h };
			auto blocIndex = scene->tiles[c];
			if (blocIndex > '9')
			{
				blocIndex -= 88;
			}
			else
			{
				blocIndex -= 49;
			}
			Renderer::CopySurfaceScaled(blocs[blocIndex].get(), nullptr, &gnd, &dst);
			c++;
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

void MainWorld::DetectCollisions()
{
	for (int i = 1; i < entities.size(); i++)
	{
		if (Collision(entities[0]->GetPosition(), *entities[0]->GetRectPtr(), entities[i]->GetPosition(), *entities[i]->GetRectPtr()))
		{
			Event e = Event(EventEnum::COLLISION, static_cast<void*>(entities[0].get()));
			EventSystem::Launch(&e);
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
	DetectCollisions();
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