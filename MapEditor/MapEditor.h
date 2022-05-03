#pragma once

#include <tuple>
#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <stack>


#include "../InputEventsEnum.h"
#include "../Position.h"
#include "../Texture.h"
#include "../Tools/Map.h"
#include "../Tools/Font.h"
#include "../IniReader.h"
#include "../Tools/Quadtree.h"
#include "SearchBar.h"


struct Rect;

class MapEditor
{
public:
	MapEditor(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned int tileSize, const std::string output_path);
	MapEditor(const std::string output_path);
	~MapEditor();

	void ProcessInput();
	void Update();
	void Draw();

	void FillMap();
	void Place4Tiles(int, int);

	bool running = true;

private:
	void InitKeyToFunction();
	void QuitApp();
	void CreeQuadrillage();
	void DessineQuadrillage();
	void ZoomOut();
	void ZoomIn();
	void ToggleQuadrillage();
	void PlaceGroundTile(Position worldPos);
	void ActiveCtrl();
	void DesactiveCtrl();
	void Ctrl_Z();
	void Sauvegarder();
	void LoadGround();
	void LoadThisObstacle(const std::string& searched_content);
	void PlaceEntity(const Position& worldPos);
	void LoadAllObstacles();
	void SetPlayerSpawnPos(const Position& worldPos);

private:
	std::unique_ptr<IniReader> ini_reader = std::make_unique<IniReader>();
	std::unique_ptr<Font> font;

	int map_width;
	int map_height;
	int tile_size;

	const std::string outputPath;

	double zoomCoef = 1;
	double zoomPas = 0.2;

	std::unique_ptr<Map> outputMap;
	std::map<std::string, std::vector<std::pair<int, int>>> outputObstacles;

	std::pair<bool, bool> mouseClickState;
	Position mousePosDifference{ 0 };
	Position currentMousePos;
	Position oldMousePos;
	Position screenMapOrigin{ 0 };


	//En lien avec le quadrillage, vecteur de points placés entre chaque case de sol
	std::vector<Position> quadrillage = {};
	std::vector<Position> resetQuadrillage = {};
	bool shouldDrawQuadrillage = false;


	//Associe a chaque touche une fonction a lancer
	std::unique_ptr<std::map<InputEventEnum, void (MapEditor::*)()>> keyToFunction;


	//Sol où on place les tiles
	std::unique_ptr<Surface> groundSurface;
	//Sol qu'on dessine a l'ecran
	std::unique_ptr<Texture> ground;
	//Pour pouvoir ctrl-z
	std::stack<std::pair<char, intptr_t>> previousPlacements;
	
	char selectedGroundBlockLetter = char(41);
	std::shared_ptr<Surface> selectedGroundBlock;

	bool groundTile_mode = true;

	Quadtree entities;
	std::string selectedEntityPath;
	std::stack<std::string> oldSelectedEntityPaths;
	std::string selectedEntityImgPath;
	std::shared_ptr<Surface> selectedEntityImg = std::make_shared<Surface>();

	std::shared_ptr<Texture> playerImg;
	Rect playerSpawnPos = Rect(0, 0, 0, 0);

	std::shared_ptr<Texture> cursorImage;
	Rect cursor_rect = Rect(0, 0, 0, 0);

	std::unique_ptr<SearchBar> searchBar;

	bool ctrl_pressed = false;
	bool should_fill = false;

	bool is_saved = false;

private:
	void type_key_backspace();  void type_key_point();  void type_key_a(); void type_key_b(); void type_key_c(); void type_key_d(); void type_key_e(); void type_key_f(); void type_key_g(); void type_key_h(); void type_key_i(); void type_key_j(); void type_key_k(); void type_key_l(); void type_key_m(); void type_key_n(); void type_key_o(); void type_key_p(); void type_key_q(); void type_key_r(); void type_key_s(); void type_key_t(); void type_key_u(); void type_key_v(); void type_key_w(); void type_key_x(); void type_key_y(); void type_key_z(); void type_key_1(); void type_key_2(); void type_key_3(); void type_key_4(); void type_key_5(); void type_key_6(); void type_key_7(); void type_key_8(); void type_key_9(); void type_key_0(); void type_key_enter();
};
