
#include <iostream>
#include <vector>
#include <cassert>

#include <thread>


#include "MapEditor.h"
#include "..\Renderer.h"
#include "..\Input.h"
#include "..\Rect.h"
#include "..\Utility.h"
#include "..\Resolution.h"
#include "..\TextureManager.h"
#include "..\Tools\Map.h"
#include "..\Tools\Font.h"
#include "../Surface.h"
#include "../Texture.h"
#include "..\Tools\Cardinaux.h"
#include "SearchBar.h"


MapEditor::MapEditor(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned int tileSize, const std::string output_path)
	: tile_size(tileSize)
	, map_width(mapWidth - (mapWidth % tileSize))
	, map_height(mapHeight - (mapHeight % tileSize))
	, outputPath(output_path)
	, entities(Quadtree(Rect(0, 0, map_width, map_height)))
{
	Renderer::Init("Kaki 2 map editor");

	InitKeyToFunction();
	CreeQuadrillage();

	outputMap = std::make_unique<Map>(map_width, map_height, tile_size, tile_size, (map_width / tile_size) * (map_height / tile_size));

	groundSurface = std::make_unique<Surface>(map_width, map_height);
	Renderer::ClearSurface(groundSurface.get(), 0, 0, 0);
	ground = std::make_unique<Texture>(*Renderer::GetRenderer(), *groundSurface->GetRendererSurface());
	
	selectedGroundBlock = TextureManager::GetSurface("../Assets/Sol/41.png");
	cursorImage = TextureManager::GetTexture("../Assets/Sol/41.png");
	cursor_rect.w = tile_size;
	cursor_rect.h = tile_size;
	Renderer::ShowCursor(true);
		
	font = std::make_unique<Font>("../Assets/Fonts/arial.ttf", 36);
	searchBar = std::make_unique<SearchBar>(font.get());

	playerImg = TextureManager::GetTexture("../Assets/MC/down/1.png");
}

MapEditor::MapEditor(const std::string output_path)
	: outputPath(output_path)
{
	outputMap = std::make_unique<Map>();

	std::string path = std::string("../MapEditor-output/") + outputPath + std::string(".bin");
	MapFile lecture(path.c_str(), std::ios::binary);
	std::string kaki_path = std::string("../MapEditor-output/") + outputPath + std::string(".kaki");
	MapFile fichier_kaki(kaki_path.c_str(), std::ios::binary);

	if (not (lecture.is_open() and fichier_kaki.is_open()))
	{
		std::cout << "Erreur de lecture : le fichier \"" << output_path << "\" est introuvable." << std::endl;
		QuitApp();
	}
	else
	{
		lecture >> *outputMap;
		lecture.close();
		outputMap->tiles.erase(0, 4);

		tile_size = outputMap->tile_w;
		map_width = outputMap->size_x - (outputMap->size_x % tile_size);
		map_height = outputMap->size_y - (outputMap->size_y % tile_size);

		fichier_kaki >> outputObstacles;
		fichier_kaki.close();

		entities = Quadtree(Rect(0, 0, map_width, map_height));

		Renderer::Init("Kaki 2 map editor");

		groundSurface = std::make_unique<Surface>(map_width, map_height);
		Renderer::ClearSurface(groundSurface.get(), 0, 0, 0);
		ground = std::make_unique<Texture>(*Renderer::GetRenderer(), *groundSurface->GetRendererSurface());

		LoadAllObstacles();
		LoadGround();

		selectedGroundBlock = TextureManager::GetSurface("../Assets/Sol/41.png");
		selectedEntityImg.reset();
		selectedEntityImgPath = "";
		groundTile_mode = true;
		cursorImage = TextureManager::GetTexture("../Assets/Sol/41.png");
		cursor_rect.w = tile_size;
		cursor_rect.h = tile_size;
		Renderer::ShowCursor(true);

		font = std::make_unique<Font>("../Assets/Fonts/arial.ttf", 36);
		searchBar = std::make_unique<SearchBar>(font.get());

		InitKeyToFunction();
		CreeQuadrillage();
		playerImg = TextureManager::GetTexture("../Assets/MC/down/1.png");
	}
}

MapEditor::~MapEditor()
{

}

void MapEditor::LoadGround()
{
	//Pas tres opti ? doit charger chaque tile � chaque fois..

	int compt{ 0 };
	for (int y = 0; y < map_height; y += tile_size)
	{
		for (int x{ 0 }; (x < map_width) and (compt < outputMap->tiles.length()); x += tile_size)
		{
			char c = outputMap->tiles[compt];
			if (c >= '(')
			{
				Rect dst = { x,y,tile_size,tile_size };
				std::string path = std::string("../Assets/Sol/") + std::to_string(c) + std::string(".png");
				auto tile = Renderer::SurfaceLoadImage(path);

				Renderer::CopySurfaceScaled(tile.get(), nullptr, groundSurface.get(), &dst);
			}
			compt++;
		}
	}
	ground = std::make_unique<Texture>(*Renderer::GetRenderer(), *groundSurface->GetRendererSurface());
}

void MapEditor::LoadAllObstacles()
{
	//Doit remplir le quadtree entities � partir de outputObstacles qui a �t� lu

	for (auto tup : outputObstacles)
	{
		if (tup.first == "") continue;
		auto copy = tup.first;
		//On supprime "./Content/Entities/" pour ne garder que "tree.ini" et pouvoir l'envoyer dans LoadThisObstacle
		copy.erase(0, 19);
		std::cout << copy << std::endl;
		LoadThisObstacle(copy);
		for (auto vect : tup.second)
		{
			PlaceEntity(Position(vect.first, vect.second));
		}
	}
}

void MapEditor::CreeQuadrillage()
{
	for (int y = 0; y <= map_height; y += tile_size)
	{
		for (int x = 0; x <= map_width; x += tile_size)
		{
			resetQuadrillage.push_back(Position(x, y));
		}
	}
}


void MapEditor::QuitApp()
{
	running = false;
}

void MapEditor::InitKeyToFunction()
{
	std::vector<InputEventEnum> keys{
		InputEventEnum::QUIT,
		InputEventEnum::ESC_down,
		InputEventEnum::SCROLL_DOWN,
		InputEventEnum::SCROLL_UP,
		InputEventEnum::down_tab,
		InputEventEnum::CTRL_down,
		InputEventEnum::CTRL_up,
		
		InputEventEnum::BACKSPACE_down, InputEventEnum::point_down, InputEventEnum::A_down, InputEventEnum::B_down, InputEventEnum::C_down, InputEventEnum::D_down, InputEventEnum::E_down, InputEventEnum::F_down, InputEventEnum::G_down, InputEventEnum::H_down, InputEventEnum::I_down, InputEventEnum::J_down, InputEventEnum::K_down, InputEventEnum::L_down, InputEventEnum::M_down, InputEventEnum::N_down, InputEventEnum::O_down, InputEventEnum::P_down, InputEventEnum::Q_down, InputEventEnum::R_down, InputEventEnum::S_down, InputEventEnum::T_down, InputEventEnum::U_down, InputEventEnum::V_down, InputEventEnum::W_down, InputEventEnum::X_down, InputEventEnum::Y_down, InputEventEnum::Z_down, InputEventEnum::down_1, InputEventEnum::down_2, InputEventEnum::down_3, InputEventEnum::down_4, InputEventEnum::down_5, InputEventEnum::down_6, InputEventEnum::down_7, InputEventEnum::down_8, InputEventEnum::down_9, InputEventEnum::down_0, InputEventEnum::down_enter,
	};
	std::vector<void (MapEditor::*)()> functions{
		&MapEditor::QuitApp,
		&MapEditor::QuitApp,
		&MapEditor::ZoomOut,
		&MapEditor::ZoomIn,
		&MapEditor::ToggleQuadrillage,
		&MapEditor::ActiveCtrl,
		&MapEditor::DesactiveCtrl,

		&MapEditor::type_key_backspace, &MapEditor::type_key_point, &MapEditor::type_key_a, &MapEditor::type_key_b, &MapEditor::type_key_c, &MapEditor::type_key_d, &MapEditor::type_key_e, &MapEditor::type_key_f, &MapEditor::type_key_g, &MapEditor::type_key_h, &MapEditor::type_key_i, &MapEditor::type_key_j, &MapEditor::type_key_k, &MapEditor::type_key_l, &MapEditor::type_key_m, &MapEditor::type_key_n, &MapEditor::type_key_o, &MapEditor::type_key_p, &MapEditor::type_key_q, &MapEditor::type_key_r, &MapEditor::type_key_s, &MapEditor::type_key_t, &MapEditor::type_key_u, &MapEditor::type_key_v, &MapEditor::type_key_w, &MapEditor::type_key_x, &MapEditor::type_key_y, &MapEditor::type_key_z, &MapEditor::type_key_1, &MapEditor::type_key_2, &MapEditor::type_key_3, &MapEditor::type_key_4, &MapEditor::type_key_5, &MapEditor::type_key_6, &MapEditor::type_key_7, &MapEditor::type_key_8, &MapEditor::type_key_9, &MapEditor::type_key_0, &MapEditor::type_key_enter,
	};

	assert(keys.size() == functions.size());

	keyToFunction = std::make_unique<std::map<InputEventEnum, void (MapEditor::*)()>>();

	for (int i = 0; i < keys.size(); i++)
	{
		keyToFunction->emplace(keys[i],functions[i]);
	}
}

void MapEditor::ToggleQuadrillage()
{
	shouldDrawQuadrillage = not shouldDrawQuadrillage;
}

void MapEditor::DessineQuadrillage()
{
	if (!shouldDrawQuadrillage) return;
	Renderer::DrawPoints(quadrillage, 255, 50, 50, 50);
}


void MapEditor::ZoomOut()
{
	zoomCoef -= zoomPas;
	Clamp(zoomCoef, 0.1, 2.0);
}

void MapEditor::ZoomIn()
{
	zoomCoef += zoomPas;
	Clamp(zoomCoef, 0.1, 2.0);
}

void MapEditor::PlaceGroundTile(Position worldPos)
{
	if (not groundTile_mode) return;

	//Si le clic est en dehors de la map on fait rien
	if ((worldPos.x >= map_width) or (worldPos.y >= map_height) or (worldPos.x < 0) or (worldPos.y < 0)) return;

	int emplacement = (worldPos.y / tile_size) * (map_width / tile_size) + (worldPos.x / tile_size);

	//Si l'emplacement est invalide ou si une tile a deja ete placee ici on fait rien
	if ((emplacement >= outputMap->tiles.length()) or (emplacement < 0)) return;
	if (outputMap->tiles[emplacement] == selectedGroundBlockLetter) return;

	is_saved = false;

	//On place l'ancienne liste de char dans la pile de tiles
	previousPlacements.emplace(outputMap->tiles[emplacement], emplacement);

	//Mettre a jour l'Output du programme pour retenir l'emplacement du nouveau sol
	outputMap->tiles[emplacement] = selectedGroundBlockLetter;

	//Dessiner la nouvelle tile de sol sur la surface
	Rect rdst = Rect(worldPos.x, worldPos.y, tile_size, tile_size);
	Renderer::CopySurfaceScaled(selectedGroundBlock.get(), NULL, groundSurface.get(), &rdst);

	//On met a jour la texture du sol a dessiner a partir de la surface
	ground.reset(new Texture(*Renderer::GetRenderer(), groundSurface.get()));
}


void MapEditor::PlaceEntity(const Position& worldPos)
{
	//Si le clic est en dehors de la map on fait rien
	if ((worldPos.x >= (map_width - cursor_rect.w)) or (worldPos.y >= (map_height - cursor_rect.h / 2)) or (worldPos.x < 0) or (worldPos.y < 0))
		return;

	//Le joueur doit etre unique, ce n'est pas un obstacle
	if (selectedEntityPath == "./Content/Entities/mc.ini") return SetPlayerSpawnPos(worldPos);

	//Si un obstacle a deja ete place ici on fait rien
	Position pos = worldPos;
	if (entities.Find(pos) != nullptr) return;

	oldSelectedEntityPaths.push(selectedEntityPath);
	is_saved = false;

	//Mettre a jour l'Output du programme pour retenir l'emplacement du nouveau sol
	outputObstacles[selectedEntityPath].push_back(std::make_pair(worldPos.x, worldPos.y));


	Rect entity_rect = Rect(worldPos.x, worldPos.y, cursor_rect.w, cursor_rect.h);
	std::shared_ptr<TextureRectMeta> p = std::make_shared<TextureRectMeta>(TextureManager::GetTexture(selectedEntityImgPath), entity_rect);
	//Inserer le pair au bon endroit dans l'arbre
	auto pushed_ptr = entities.Push(p);


	//Sauvegarder : avoir un .first de -1 signifie que que c'est un obstacle avec comme .second le pointeur de l'obstacle place dans le quadtree
	previousPlacements.push(std::make_pair(-1, reinterpret_cast<intptr_t>(pushed_ptr)));
}

void MapEditor::SetPlayerSpawnPos(const Position& worldPos)
{
	playerSpawnPos = Position(int((cursor_rect.x - int(screenMapOrigin.x))/zoomCoef), int((cursor_rect.y - int(screenMapOrigin.y))/zoomCoef));
}

void MapEditor::ActiveCtrl()
{
	ctrl_pressed = true;
}

void MapEditor::DesactiveCtrl()
{
	ctrl_pressed = false;
}


void MapEditor::Place4Tiles(int x, int y)
{
	Position tilePos = { x, y };
	PlaceGroundTile(tilePos);

	tilePos = { map_width - tile_size - x, y };
	PlaceGroundTile(tilePos);

	tilePos = { x, map_height - tile_size - y };
	PlaceGroundTile(tilePos);

	tilePos = { map_width - tile_size - x, map_height - tile_size - y };
	PlaceGroundTile(tilePos);
}

void MapEditor::FillMap()
{
	if (not groundTile_mode) return;
	if (!should_fill) return;
	static auto y = 0, x = 0;

	Place4Tiles(x, y);

	x += tile_size;
	if (x > map_width / 2)
	{
		x = 0;
		y += tile_size;
		if (y > map_height / 2)
		{
			should_fill = false;
			x = 0;
			y = 0;
		}
	}
}

void MapEditor::Ctrl_Z()
{
	if (previousPlacements.size() == 0) return;
	is_saved = false;

	std::pair<char, intptr_t> top = previousPlacements.top();
	previousPlacements.pop();

	if (top.first == -1)
	{
		auto ptr = reinterpret_cast<Quadtree_node*>(top.second);
		entities.Remove(ptr);
		auto index_recherche = oldSelectedEntityPaths.top();
		oldSelectedEntityPaths.pop();
		outputObstacles[index_recherche].pop_back();
		return;
	}

	std::shared_ptr<Surface> tmpGround;
	
	tmpGround = Renderer::SurfaceLoadImage(std::string("../Assets/Sol/") + std::to_string(int(top.first)) + std::string(".png"));
	outputMap->tiles[top.second] = top.first;

	//Ce calcul donne des coordonnees de tile a partir d'un index de string
	Rect rdst = Rect(int((top.second % (map_width/tile_size)) * tile_size), int((top.second / (map_width/tile_size)) * tile_size), tile_size, tile_size);
	Renderer::CopySurfaceScaled(tmpGround.get(), NULL, groundSurface.get(), &rdst);
	ground.reset(new Texture(*Renderer::GetRenderer(), groundSurface.get()));
}

void MapEditor::Sauvegarder()
{
	if (is_saved) return;

	//On sauvegarde le .bin pour l'emplacement des tiles
	std::string path = std::string("../MapEditor-output/") + outputPath + std::string(".bin");
	std::ofstream output(path.c_str(), std::ios::binary);
	
	outputMap->player_spawn_x = playerSpawnPos.x;
	outputMap->player_spawn_y = playerSpawnPos.y;

	output << *outputMap;
	output.close();

	
	//On sauvegarde le .kaki pour l'emplacement des obstacles
	std::string kaki_path = std::string("../MapEditor-output/") + outputPath + std::string(".kaki");

	std::ofstream kaki_output(kaki_path.c_str(), std::ios::binary);

	kaki_output << outputObstacles;
	kaki_output.close();

	std::cout << "Enregistrement reussi. Vous pouvez trouver les deux fichiers de sortie dans le repertoire \"" << path << " et " << kaki_path << "\"." << std::endl;

	is_saved = true;
}

void MapEditor::type_key_enter()
{
	if (searchBar->IsEmpty()) return;
	auto searched_content = searchBar->GetContent();
	searchBar->Erase();

	std::array<std::string, 4> voisinsPossibles = { "haut", "gauche", "bas", "droite" };
	auto cherche = std::find(voisinsPossibles.begin(), voisinsPossibles.end(), searched_content);
	if(cherche != voisinsPossibles.end())
	{
		//Faire un std::cin pour r�cuperer le nom du fichier de la map voisine
		//ce if se d�clenche quand l'utilisateur a �crit "gauche" dans la barre, ou droite, haut etc
		std::string rep;
		do
		{
			std::cout << "Quelle map est voisine (" << searched_content << ") ?" << std::endl;
			std::cin >> rep;
		}while(std::cin.fail() or (rep == "") or (rep.length() >= 10));

		strcpy_s(outputMap->voisins[int(cherche - voisinsPossibles.begin())], sizeof(char[10]),rep.c_str());
		return;
	}

	auto ground_path = "../Assets/Sol/" + searched_content;

	auto tmp = std::move(selectedGroundBlock);
	auto tmp2 = std::move(cursorImage);
	try
	{
		selectedGroundBlock = Renderer::SurfaceLoadImage(ground_path);

		selectedGroundBlockLetter = char(StrToInt(searched_content));

		cursorImage = std::make_shared<Texture>(*Renderer::GetRenderer(), selectedGroundBlock.get());
		if (cursorImage.get() == nullptr) throw;

		groundTile_mode = true;
		cursor_rect.w = tile_size;
		cursor_rect.h = tile_size;
		std::cout << "selected letter : " << selectedGroundBlockLetter << std::endl;

	}
	catch (const std::exception e)
	{
		//Ici faire en sorte que ca affiche un message d'erreur sur l'ecran disant que l'image n'a pas ete trouvee

		selectedGroundBlock.reset();
		selectedGroundBlock = std::move(tmp);

		cursorImage.reset();
		cursorImage = std::move(tmp2);

		std::cout << "Error : " << e.what() << std::endl;
	}
	catch (...)
	{
		//Ici faire en sorte que ca affiche un message d'erreur sur l'ecran disant que l'image n'a pas ete trouvee

		selectedGroundBlock.reset();
		selectedGroundBlock = std::move(tmp);

		cursorImage.reset();
		cursorImage = std::move(tmp2);

		//Ici le sol n'a pas ete trouve donc on cherche un obstacle
		try
		{
			LoadThisObstacle(searched_content);
		}
		catch (const char* e)
		{
			std::cout << e << std::endl;
		}
	}
}

void MapEditor::LoadThisObstacle(const std::string& searched_content)
{
	try
	{
		auto path = "../Content/Entities/" + searched_content;
		std::ifstream iniFile(path);
		if (not iniFile.is_open()) throw("");

		std::map<std::string, std::map<std::string, std::string>> iniOutput;
		ini_reader->ParseIni(iniFile, iniOutput);

		try
		{
			std::string imgPath = "../" + iniOutput["StaticDraw"]["path"];

			selectedEntityImg = Renderer::SurfaceLoadImage(imgPath);
			//Ici �a throw quand l'entity n'a pas de StaticDraw : donc quand on cherche une animation.
			selectedEntityPath = path.erase(0, 1);
			selectedEntityImgPath = imgPath;

			cursor_rect.w = StrToInt(iniOutput["StaticDraw"]["width"]);
			cursor_rect.h = StrToInt(iniOutput["StaticDraw"]["height"]);

			cursorImage.reset();
			cursorImage = TextureManager::GetTexture(imgPath);
		}
		catch(...)
		{
			std::string imgPath;
			if (searched_content == "mc.ini")
			{
				imgPath = "../" + iniOutput["Animation"]["path"] + "/down/1.png";
				std::cout << imgPath << std::endl;
				selectedEntityImg = Renderer::SurfaceLoadImage(imgPath);
				//Ici �a throw pas normalement

				selectedEntityPath = path.erase(0, 1);
				selectedEntityImgPath = imgPath;

				cursor_rect.w = StrToInt(iniOutput["Animation"]["width"]);
				cursor_rect.h = StrToInt(iniOutput["Animation"]["height"]);

				playerSpawnPos.w = cursor_rect.w;
				playerSpawnPos.h = cursor_rect.h;
			}
			else
			{
				//On cherche une animation
				imgPath = "../" + iniOutput["ObstacleAnimation"]["path"] + "/1.png";
				std::cout << imgPath << std::endl;
				selectedEntityImg = Renderer::SurfaceLoadImage(imgPath);
				//Ici �a throw pas normalement

				selectedEntityPath = path.erase(0, 1);
				selectedEntityImgPath = imgPath;

				cursor_rect.w = StrToInt(iniOutput["ObstacleAnimation"]["width"]);
				cursor_rect.h = StrToInt(iniOutput["ObstacleAnimation"]["height"]);
			}
			cursorImage.reset();
			cursorImage = TextureManager::GetTexture(imgPath);
		}

		groundTile_mode = false;
		shouldDrawQuadrillage = false;
		selectedGroundBlockLetter = -1;
		selectedGroundBlock.reset();

	}
	catch (...)
	{
		std::cout << "Une erreur s'est produite en essayant de charger le fichier \"" + searched_content + std::string("\". Merci de reessayer.");
	}
}


void MapEditor::ProcessInput()
{
	//Mise a jour des clicks et de la position du curseurs de souris

	mouseClickState = Input::GetMouseState(&currentMousePos.x, &currentMousePos.y);

	if (mouseClickState.second)
	{
		mousePosDifference = currentMousePos - oldMousePos;
		screenMapOrigin += mousePosDifference;
		Clamp(screenMapOrigin.x, int(-map_width * zoomCoef), RES_X);
		Clamp(screenMapOrigin.y, int(-map_height * zoomCoef), RES_Y);
	}
	oldMousePos = currentMousePos;

	if (mouseClickState.first)
	{
		Position worldCursorPos = (currentMousePos - screenMapOrigin) * (1 / zoomCoef);
		//std::cout << worldCursorPos.x << ", " << worldCursorPos.y << std::endl;
		if (groundTile_mode)
		{
			PlaceGroundTile((worldCursorPos / tile_size) * tile_size);
		}
		else
		{
			PlaceEntity(((worldCursorPos - Position(tile_size / 2, tile_size / 2)) / tile_size) * tile_size);
		}
	}



	//Clavier et scroll de souris
	InputEventEnum nextEvent;
	while (Input::GetNextInputEvent(&nextEvent))
	{
		//std::cout << int(nextEvent) << std::endl;
		auto funcPtr = keyToFunction->find(nextEvent);
		if (funcPtr != keyToFunction->end())
		{
			void (MapEditor:: * f)() = funcPtr->second;
			(this->*f)();
		}
	}
}

void MapEditor::Update()
{
	//std::cout << screenMapOrigin.x << ", " << screenMapOrigin.y << std::endl;
	quadrillage = resetQuadrillage;
	for (auto& pos : quadrillage)
		pos = pos * zoomCoef + screenMapOrigin;
}

void MapEditor::Draw()
{
	Renderer::Clear(173, 201, 224);

	//Sol
	auto r = Rect(screenMapOrigin.x, screenMapOrigin.y, int(map_width * zoomCoef), int(map_height * zoomCoef));
	Renderer::FullBlit(ground.get(), r);

	//Si le mode "shouldDrawQuadrillage" est actif on n'affiche pas les obstacles
	if (shouldDrawQuadrillage)
	{
		DessineQuadrillage();
	}
	else
	{
		//Pour afficher les obstacles on itere a travers le quadtree contenant tous les obstacles
		//Grosse triche : on voit pas les obstacles disparaitre en sortant de l'�cran juste parce que l'�cran est 4 fois plus grand lol
		Rect tmp_rect;
		entities.Parcours_Infixe_Collision(Rect(-screenMapOrigin.x - RES_X, -screenMapOrigin.y - RES_Y, int((int(4 * RES_X)) / zoomCoef), int((int(4 * RES_Y)) / zoomCoef)),
			[&](Quadtree_node* e)
			{
				tmp_rect = { int(e->val->get_x() * zoomCoef + screenMapOrigin.x), int(e->val->get_y() * zoomCoef + screenMapOrigin.y), int(e->val->get_w() * zoomCoef), int(e->val->get_h() * zoomCoef) };
				Renderer::FullBlit(e->val->get_imgptr(), tmp_rect);
			}
		);
	}
	
	Rect playerRect = Rect(int(playerSpawnPos.x * zoomCoef) + screenMapOrigin.x, int(playerSpawnPos.y * zoomCoef) + screenMapOrigin.y, int(playerSpawnPos.w * zoomCoef), int(playerSpawnPos.h * zoomCoef));
	Renderer::FullBlit(playerImg.get(), playerRect);
	searchBar->Draw();

	//Curseur
	if (groundTile_mode)
	{
		cursor_rect.x = int(currentMousePos.x - tile_size * zoomCoef / 2);
		cursor_rect.y = int(currentMousePos.y - tile_size * zoomCoef / 2);
	}
	else
	{
		cursor_rect.x = int(currentMousePos.x - cursor_rect.w * zoomCoef / 2);
		cursor_rect.y = int(currentMousePos.y - cursor_rect.h * zoomCoef / 2);
	}
	
	FillMap();
	auto rr = Rect(cursor_rect.x, cursor_rect.y, int(cursor_rect.w * zoomCoef), int(cursor_rect.h * zoomCoef));
	Renderer::FullBlit(cursorImage.get(), rr);
}


//Gestion penible de chaque touche du clavier
#if true

void MapEditor::type_key_backspace()
{
	searchBar->BackSpace();
}

void MapEditor::type_key_point()
{
	searchBar->AddLetter('.');
}

void MapEditor::type_key_a()
{
	if (ctrl_pressed)
	{
		should_fill = true;
	}
	else
	{
		searchBar->AddLetter('a');
	}
}

void MapEditor::type_key_b()
{
	searchBar->AddLetter('b');
}

void MapEditor::type_key_c()
{
	searchBar->AddLetter('c');
}

void MapEditor::type_key_d()
{
	searchBar->AddLetter('d');
}

void MapEditor::type_key_e()
{
	searchBar->AddLetter('e');
}

void MapEditor::type_key_f()
{
	searchBar->AddLetter('f');
}

void MapEditor::type_key_g()
{
	searchBar->AddLetter('g');
}

void MapEditor::type_key_h()
{
	searchBar->AddLetter('h');
}

void MapEditor::type_key_i()
{
	searchBar->AddLetter('i');
}

void MapEditor::type_key_j()
{
	searchBar->AddLetter('j');
}

void MapEditor::type_key_k()
{
	searchBar->AddLetter('k');
}

void MapEditor::type_key_l()
{
	searchBar->AddLetter('l');
}

void MapEditor::type_key_m()
{
	searchBar->AddLetter('m');
}

void MapEditor::type_key_n()
{
	searchBar->AddLetter('n');
}

void MapEditor::type_key_o()
{
	searchBar->AddLetter('o');
}

void MapEditor::type_key_p()
{
	searchBar->AddLetter('p');
}

void MapEditor::type_key_q()
{
	searchBar->AddLetter('q');
}

void MapEditor::type_key_r()
{
	searchBar->AddLetter('r');
}

void MapEditor::type_key_s()
{
	if (ctrl_pressed)
	{
		return Sauvegarder();
	}
	searchBar->AddLetter('s');
}

void MapEditor::type_key_t()
{
	searchBar->AddLetter('t');
}

void MapEditor::type_key_u()
{
	searchBar->AddLetter('u');
}

void MapEditor::type_key_v()
{
	searchBar->AddLetter('v');
}

void MapEditor::type_key_w()
{
	searchBar->AddLetter('w');
}

void MapEditor::type_key_x()
{
	searchBar->AddLetter('x');
}

void MapEditor::type_key_y()
{
	searchBar->AddLetter('y');
}

void MapEditor::type_key_z()
{
	if (ctrl_pressed)
	{
		Ctrl_Z();
	}
	else
	{
		searchBar->AddLetter('z');
	}
}

void MapEditor::type_key_1()
{
	searchBar->AddLetter('1');
}

void MapEditor::type_key_2()
{
	searchBar->AddLetter('2');
}

void MapEditor::type_key_3()
{
	searchBar->AddLetter('3');
}

void MapEditor::type_key_4()
{
	searchBar->AddLetter('4');
}

void MapEditor::type_key_5()
{
	searchBar->AddLetter('5');
}

void MapEditor::type_key_6()
{
	searchBar->AddLetter('6');
}

void MapEditor::type_key_7()
{
	searchBar->AddLetter('7');
}

void MapEditor::type_key_8()
{
	searchBar->AddLetter('8');
}

void MapEditor::type_key_9()
{
	searchBar->AddLetter('9');
}

void MapEditor::type_key_0()
{
	searchBar->AddLetter('0');
}

#endif