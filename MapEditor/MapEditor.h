#pragma once

#include "../InputEventsEnum.h"
#include "../Position.h"
#include "../Texture.h"
#include "../Tools/Map.h"

#include <tuple>
#include <vector>
#include <memory>
#include <map>

struct Rect;

class MapEditor
{
public:
	MapEditor(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned int tileSize);
	~MapEditor();

	void ProcessInput();
	void Update();
	void Draw();

private:
	void InitKeyToFunction();
	void QuitApp();
	void CreeQuadrillage();
	void DessineQuadrillage();
	void ZoomOut();
	void ZoomIn();
	void ToggleQuadrillage();
	void PlaceGroundTile(Position worldPos);

public:
	bool running = true;

private:
	const int map_width;
	const int map_height;
	const int tile_size;

	double zoomCoef = 1;
	double zoomPas = 0.2;

	std::unique_ptr<Map> outputMap;


	std::pair<bool, bool> mouseClickState;
	Position mousePosDifference{ 0 };
	Position currentMousePos;
	Position oldMousePos;
	Position screenMapOrigin{ 0 };


	//En lien avec le quadrillage, vecteur de points placés entre chaque case de sol
	std::vector<Position> quadrillage = {};
	std::vector<Position> resetQuadrillage = {};
	bool shouldDrawQuadrillage = true;


	//Associe a chaque touche une fonction a lancer
	std::unique_ptr<std::map<InputEventEnum, void (MapEditor::*)()>> keyToFunction;


	//Sol où on place les tiles
	std::unique_ptr<Surface> groundSurface;
	//Sol qu'on dessine a l'ecran
	std::unique_ptr<Texture> ground;

	char selectedGroundBlockLetter = 'a';
	std::shared_ptr<Surface> selectedGroundBlock;

	std::shared_ptr<Texture> cursorImage;
};
