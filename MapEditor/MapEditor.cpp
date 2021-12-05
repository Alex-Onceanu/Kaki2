#include "MapEditor.h"
#include "..\Renderer.h"
#include "..\Input.h"
#include "..\Rect.h"
#include "..\Utility.h"
#include "..\Resolution.h"
#include "..\TextureManager.h"
#include "..\Tools\Map.h"

#include <iostream>
#include <vector>
#include <cassert>


MapEditor::MapEditor(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned int tileSize)
	: tile_size(tileSize)
	, map_width(mapWidth - (mapWidth % tileSize))
	, map_height(mapHeight - (mapHeight % tileSize))
{
	Renderer::Init("Kaki 2 map editor");

	InitKeyToFunction();
	CreeQuadrillage();

	outputMap = std::make_unique<Map>(map_width, map_height, tile_size, tile_size, (map_width / tile_size) * (map_height / tile_size));

	groundSurface = std::make_unique<Surface>(map_width, map_height);
	Renderer::ClearSurface(groundSurface.get(), 0, 0, 0);
	ground = std::make_unique<Texture>(*Renderer::GetRenderer(), *groundSurface->GetRendererSurface());
	
	selectedGroundBlock = TextureManager::GetSurface("../Assets/Sol/f.png");
	cursorImage = TextureManager::GetTexture("../Assets/Sol/f.png");
	Renderer::ShowCursor(false);
}

MapEditor::~MapEditor()
{
	
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
		InputEventEnum::Q_down,
		InputEventEnum::ESC_down,
		InputEventEnum::SCROLL_DOWN,
		InputEventEnum::SCROLL_UP,
	};
	std::vector<void (MapEditor::*)()> functions{
		&MapEditor::QuitApp,
		&MapEditor::ToggleQuadrillage,
		&MapEditor::QuitApp,
		&MapEditor::ZoomOut,
		&MapEditor::ZoomIn,
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

#if 1

void MapEditor::DessineQuadrillage()
{
	if (!shouldDrawQuadrillage) return;
	Renderer::DrawPoints(quadrillage, 255, 50, 50, 50);
}

#else
void MapEditor::DessineQuadrillage()
{
	if (!shouldDrawQuadrillage) return;

	std::vector<Position> pts;

	int MH = map_height * zoomCoef,
		MW = map_width * zoomCoef,
		step = tile_size * zoomCoef;

	// draw horz
	for (auto i = 0; i <= MH; i += step)
	{
		pts.push_back({0, i});
		pts.push_back({ MW, i });
		pts.push_back({ 0, i });
	}


	// draw verz
	for (auto i = 0; i <= MW; i += step)
	{
		pts.push_back({ i, 0});
		pts.push_back({ i, MH});
		pts.push_back({ i, 0 });
	}

	auto o = screenMapOrigin + mousePosDifference;
	for (auto& pos : pts)
		pos += screenMapOrigin;

	Renderer::DrawLines(pts, 15, 15, 15, 15);

}

#endif

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
	//Mettre a jour l'Output du programme pour retenir l'emplacement du nouveau sol
	
	//Si le clic est en dehors de la map on fait rien
	if ((worldPos.x >= map_width) or (worldPos.y >= map_height) or (worldPos.x < 0) or (worldPos.y < 0)) return;

	int emplacement = (worldPos.y / tile_size) * (map_width / tile_size) + (worldPos.x / tile_size);

	//Si l'emplacement est invalide ou si une tile a deja ete placee ici on fait rien
	if ((emplacement >= outputMap->tiles.length()) or (emplacement < 0)) return;
	if (outputMap->tiles[emplacement] != '.') return;

	outputMap->tiles[emplacement] = selectedGroundBlockLetter;

	//Dessiner la nouvelle tile de sol sur la surface
	Rect rdst = Rect(worldPos.x, worldPos.y, tile_size, tile_size);
	Renderer::CopySurfaceScaled(selectedGroundBlock.get(), NULL, groundSurface.get(), &rdst);
	//On met a jour la texture du sol a dessiner a partir de la surface
	ground.reset(new Texture(*Renderer::GetRenderer(), groundSurface.get()));

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

		PlaceGroundTile((worldCursorPos / tile_size) * tile_size);
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
	auto r = Rect(screenMapOrigin.x, screenMapOrigin.y, map_width * zoomCoef, map_height * zoomCoef);
	Renderer::FullBlit(ground.get(), r);

	DessineQuadrillage();

	//Curseur
	auto rr = Rect(currentMousePos.x - tile_size * zoomCoef / 2, currentMousePos.y - tile_size * zoomCoef / 2, tile_size * zoomCoef, tile_size * zoomCoef);
	Renderer::FullBlit(cursorImage.get(), rr);
}