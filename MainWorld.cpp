#include "MainWorld.h"
#include "TextureManager.h"
#include "Input.h"
#include "InputEventsEnum.h"
#include "EventSystem.h"
#include "EventEnum.h"
#include "Camera.h"
#include "Resolution.h"
#include "Utility.h"
#include "Fader.h"
#include "Tools/Cardinaux.h"

#include "ControllerFactory.h"
#include "EntityController.h"
#include "AnimationController.h"
#include "PositionController.h"
#include "StaticDrawController.h"
#include "PlayerMovementController.h"
#include "CollisionController.h"

#include <algorithm>
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

		InputEventEnum::E_down,
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

		EventEnum::MC_TELEPORT,
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
	LoadScene("1");

	std::ifstream mc_ini("./Content/Entities/mc.ini");

	std::unique_ptr<Entity> mc = std::make_unique<Entity>();
	iniReader->Read(cf.get(), mc_ini, *mc);
	const int* w;
	const int* h;
	mc->GetSizePtr(&w, &h);
	camera = std::make_unique<Camera>(mc->GetPositionPtr(), w, h, scene->size_x, scene->size_y);
	mc->SetPosition(Position(scene->player_spawn_x, scene->player_spawn_y));
	player = mc.get();
	entities.push_back(std::move(mc));

	LoadGround();
	LoadEntities("1");

#if 0
	std::map<std::string, std::vector<std::pair<int, int>>> m;
	std::vector<std::pair<int, int>> v;
#define p(x,y) v.push_back(std::make_pair(x,y))

	p(0, 0);
	p(208 * 1, 0);
	p(208 * 2, 0);
	p(208 * 3, 0);
	p(208 * 4, 0);
	p(208 * 5, 0);
	p(208 * 6, 0);
	p(208 * 7, 0);
	p(208 * 9, 0);
	p(208 * 10, 0);
	p(208 * 11, 0);

	p(208 * 1, 1840);
	p(208 * 2, 1840);
	p(208 * 3, 1840);
	p(208 * 4, 1840);
	p(208 * 5, 1840);
	p(208 * 6, 1840);
	p(208 * 7, 1840);
	p(208 * 8, 1840);
	p(208 * 9, 1840);
	p(208 * 10, 1840);
	p(208 * 11, 1840);

	p(0, 208 * 1);
	p(0, 208 * 2);
	p(0, 208 * 3);
	p(0, 208 * 4);
	p(0, 208 * 5);
	p(0, 208 * 6);
	p(0, 208 * 7);
	p(0, 208 * 8);
	p(0, 208 * 9);

	p(2300, 208 * 1);
	p(2300, 208 * 2);
	p(2300, 208 * 3);
	p(2300, 208 * 4);
	p(2300, 208 * 5);
	p(2300, 208 * 6);
	p(2300, 208 * 7);
	p(2300, 208 * 8);
	p(2300, 208 * 9);
	p(2300, 208 * 10);

	m.emplace("./Content/Entities/tree.ini", v);
	std::ofstream fichier("./Content/Maps/test1.bin");
	fichier << m;
	fichier.close();
	throw;
#endif

	auto fader_tmp = std::make_unique<Fader>();
	this->fader = fader_tmp.get();
	entities.push_back(std::move(fader_tmp));

	/*std::srand(static_cast<int>(time(NULL)));
	for (int i = 0; i < 10; i++)
	{
		std::ifstream tree_ini("./Content/Entities/tree.ini");
		if (not tree_ini.is_open()) throw("Cannot open tree.ini");

		Position p{ std::rand() % (scene->size_x + RES_X),rand() % (scene->size_y + RES_Y) };
		std::unique_ptr<Entity> e = std::make_unique<Entity>();
		iniReader->Read(cf.get(), tree_ini, *e);
		e->SetPosition(p);
		entities.push_back(std::move(e));
	}*/
}

#if 0
void MainWorld::LoadEntities(std::string bin_map_file_path)
{
	std::string file_path = "./Content/Maps/" + bin_map_file_path + ".kaki";
	MapFile fichier_bin(file_path.c_str(), std::ios::binary);
	if (not fichier_bin.is_open()) throw("Cannot open " + file_path);

	//Associe a chaque type d'obstacle l'ensemble de ses positions sur la carte
	//map de <"nom_du_fichier .ini", liste des positions de cet obstacle>
	//Exemple : <"Arbre.ini", {(x1, y1), (x2,y2), (x3,y3)...}
	std::map<std::string, std::vector<std::pair<int, int>>> entity_to_pos;

	fichier_bin >> entity_to_pos;

	//Pour chaque obstacle
	for (auto it = entity_to_pos.begin(); it != entity_to_pos.end(); it++)
	{
		if (it->first == "") continue;
		
		std::ifstream fichier_ini_enitity_actuel(it->first);
		if (not fichier_ini_enitity_actuel.is_open()) throw("Cannot open " + it->first);

		//Pour chaque position d'un obstacle
		for (auto jt : it->second)
		{
			std::unique_ptr<Entity> e = std::make_unique<Entity>();

			iniReader->Read(cf.get(), fichier_ini_enitity_actuel, *e);
			e->SetPosition(Position({ jt.first,jt.second }));

			entities.push_back(std::move(e));

			//On revient au debut du fichier sinon reste bloque au EOF
			fichier_ini_enitity_actuel.clear();
			fichier_ini_enitity_actuel.seekg(0,std::ios::beg);
		}
	}
	std::sort(entities.begin()+1, entities.end(), [](std::unique_ptr<Entity>& a, std::unique_ptr<Entity>& b) { return a->GetPosition().y < b->GetPosition().y; });

}
#else

void MainWorld::LoadEntities(std::string bin_map_file_path)
{
	std::string file_path = "./Content/Maps/" + bin_map_file_path + ".kaki";
	MapFile fichier_bin(file_path.c_str(), std::ios::binary);
	if (not fichier_bin.is_open()) throw("Cannot open " + file_path);

	//Associe a chaque type d'obstacle l'ensemble de ses positions sur la carte
	//map de <"nom_du_fichier .ini", liste des positions de cet obstacle>
	//Exemple : <"Arbre.ini", {(x1, y1), (x2,y2), (x3,y3)...}
	std::map<std::string, std::vector<std::pair<int, int>>> entity_to_pos;

	fichier_bin >> entity_to_pos;

	//Pour chaque obstacle
	for (auto it = entity_to_pos.begin(); it != entity_to_pos.end(); it++)
	{
		if (it->first == "") continue;

		std::ifstream fichier_ini_enitity_actuel(it->first);
		if (not fichier_ini_enitity_actuel.is_open()) throw("Cannot open " + it->first);

		InitialData ini_actuel;
		iniReader->ParseIni(fichier_ini_enitity_actuel, ini_actuel.data);


		//Pour chaque position d'un obstacle
		for (auto jt : it->second)
		{
			std::unique_ptr<Entity> e = std::make_unique<Entity>();
			e->LoadInitialData(cf.get(), ini_actuel);
			e->SetPosition(Position({ jt.first,jt.second }));

			entities.push_back(std::move(e));

			//On revient au debut du fichier sinon reste bloque au EOF
			fichier_ini_enitity_actuel.clear();
			fichier_ini_enitity_actuel.seekg(0, std::ios::beg);
		}
	}
	std::sort(entities.begin() + 1, entities.end(), [](std::unique_ptr<Entity>& a, std::unique_ptr<Entity>& b) { return a->GetPosition().y < b->GetPosition().y; });

}

#endif
#if 0
void MainWorld::LoadEntities()
{
	const int map_w = scene->size_x;
	const int map_h = scene->size_y;

	std::ifstream fichier("./Content/Maps/e1.ini");
	if (not fichier.is_open()) throw("Cannot open e1.ini");

	std::map<std::string, std::map<std::string, std::string>> ini;
	iniReader->ParseIni(fichier, ini);

	const auto& map = ini["map"]["obstacles"];
	assert(map.length() != 0);

	//On fait un dictionnaire de tous les dictionnaires d'entities pour pas avoir a relire chaque fichier a chaque copie
	// <nom du fichier, fichier ini lu>
	//Utiliser des pointeurs plutot ?
	std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> inis_dict;

	int c = 0;
	for (int y = 0; y < map_h * 2; y += scene->tile_h * 2)
	{
		for (int x{ 0 }; (x < map_w * 2) and (c < map.length()); x += scene->tile_w * 2)
		{
			auto iss = IntToStr(map[c]);
			if (map[c] >= 'a')
			{
				//ini["entities"][iss] est le path de l'ini decrivant l'entity qu'on charge

				auto fichier_entity = inis_dict.find(ini["entities"][iss]);

				//Si le fichier n'a pas encore �t� lu on le lit et on le place dans la map
				if (not (fichier_entity != inis_dict.end()))
				{
					std::ifstream obst_file(ini["entities"][iss]);
					if (not obst_file.is_open()) throw(std::string("Cannot open ") + ini["entities"][iss]);

					iniReader->ParseIni(obst_file, inis_dict[ini["entities"][iss]]);

					fichier_entity = inis_dict.find(ini["entities"][iss]);
				}

				//On cr�e l'Entity a partir de la map qu'on a
				Position p{ x,y };
				std::unique_ptr<Entity> e = std::make_unique<Entity>();

				iniReader->Read(cf.get(), fichier_entity->second, *e);

				e->SetPosition(p);
				entities.push_back(std::move(e));
			}
			c++;
		}
	}
}
#endif

void MainWorld::LoadScene(std::string bin_path)
{
	scene = std::make_unique<Map>();
	std::string path;
	path = "./Content/Maps/" + bin_path + ".bin";
	try
	{
		MapFile fichier(path.c_str(), std::ios::binary);
		
		fichier >> *scene;
		RemoveZerosStr(scene->tiles);
		
	}
	catch (std::exception e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << e.what() << std::endl;
	}
}

#if 0 
//Ancien LoadGround, c'�tait quand m�me bien parce que toutes les tiles �taient charg�es une seule et unique fois au lancement

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
#else

void MainWorld::LoadGround()
{
	// Tant qu'on depasse pas la surface et qu'il reste des tiles a placer on lit le prochain bloc et on le place

	const int map_w = scene->size_x;
	const int map_h = scene->size_y;
	auto gnd = Surface(map_w, map_h);
	Rect dst;

	int c{ 0 };
	for (int y = 0; y < map_h; y += scene->tile_h)
	{
		for (int x{ 0 }; (x < map_w) and (c < scene->tiles.length()); x += scene->tile_w)
		{
			dst = { x,y,scene->tile_w,scene->tile_h };
			auto nom_du_bloc = int(scene->tiles[c]);
			
			std::string path = std::string("./Assets/Sol/") + std::to_string(nom_du_bloc) + std::string(".png");
			auto tile = Renderer::SurfaceLoadImage(path);
			Renderer::CopySurfaceScaled(tile.get(), nullptr, &gnd, &dst);
			c++;
			//ptdr c++
		}
	}
	this->ground = std::make_unique<Texture>(*Renderer::GetRenderer(), &gnd);
}

#endif

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
		if (not entities[i]->IsSolid()) continue;
		if (Collision(player->GetPosition(), *player->GetRectPtr(), entities[i]->GetPosition(), *entities[i]->GetRectPtr()))
		{
			Event e = Event(EventEnum::COLLISION, static_cast<void*>(player));
			EventSystem::Launch(&e);
		}
	}
}

void MainWorld::CheckIfShouldFade()
{
	const Position player_pos = player->GetPosition();

	//Decalage entre la limite de la map et la ou commence le fade
	const int fade_limit_starts_where = 150;

	const int min_dist_x = std::min(player_pos.x, scene->size_x - player_pos.x);
	const int min_dist_y = std::min(player_pos.y, scene->size_y - player_pos.y);

	const int min_dist = std::min(min_dist_x, min_dist_y);

	if (min_dist >= fade_limit_starts_where)
	{
		if (fader->GetAlpha() > 0)
		{
			fader->SetAlpha(MinClamp(fader->GetAlpha() - 5, 0));
		}
		else
		{
			fader->SetAlpha(0);
		}
		return;
	}

	if (min_dist <= 20)
	{
		fader->SetAlpha(255);
		return;
	}

	const int proportion_255 = min_dist * 255 / fade_limit_starts_where;
	fader->SetAlpha(255 - proportion_255);
}

void MainWorld::ChangerScene(std::string __voisin)
{
	if (__voisin == "") return;

	std::ifstream test("./Content/Maps/" + __voisin + ".kaki");
	if (not test.is_open())
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << __voisin << "n'existe pas." << std::endl;
		return;
	}

	scene.reset();
	entities.resize(1);

	LoadScene(__voisin);
	player->SetPosition(Position(scene->player_spawn_x, scene->player_spawn_y));
	//camera->SetPos(player->GetPosition());
	EventSystem::Clean();

	LoadGround();
	LoadEntities(__voisin);

	auto fader_tmp = std::make_unique<Fader>();
	this->fader = fader_tmp.get();
	entities.push_back(std::move(fader_tmp));
	fader->SetAlpha(255);
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
	//Savoir s'il faut passer a la map suivante
	if (Clamp(player->GetPositionPtr()->x, 0, scene->size_x))
	{
		//horizontal
		if (player->GetPosition().x == 0)
		{
			ChangerScene(scene->voisins[int(Cardinaux::OUEST)]);
		}
		else
		{
			ChangerScene(scene->voisins[int(Cardinaux::EST)]);
		}
	}

	else if (Clamp(player->GetPositionPtr()->y, 0, scene->size_y))
	{
		//vertical
		if (player->GetPosition().y == 0)
		{
			ChangerScene(scene->voisins[int(Cardinaux::NORD)]);
		}
		else
		{
			ChangerScene(scene->voisins[int(Cardinaux::SUD)]);
		}
	}

	for (auto&& e : entities)
	{
		e->Update();
	}
	DetectCollisions();
	//La camera suit toujours le joueur, mais doit s'arreter aux limites de la map
	camera->UpdatePosition(MinClamp(scene->size_x - RES_X, -1), MinClamp(scene->size_y - RES_Y, -1));
	
	//Fait en sorte que le joueur ne sorte pas de la map
	int* w;
	int* h;
	player->GetSizePtr(&w, &h);

	
	CheckIfShouldFade();
}

void MainWorld::Draw()
{
	//Pour du bleu marine :
	//Renderer::Clear(9, 11, 51);
	Renderer::Clear(0, 0, 0);


	Position camPos = camera->GetPos();

	Rect groundRect = { -camPos.x, -camPos.y, scene->size_x, scene->size_y };
	Renderer::FullBlit(ground.get(), groundRect);

	for (auto&& e = entities.begin() + 1; e != entities.end() - 1; e++)//: entities)
	{
		(*e)->Draw(camPos);
	}
	player->Draw(camPos);
	fader->Draw(camPos);
}