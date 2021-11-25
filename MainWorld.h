#pragma once

#include "World.h"
#include "TextureManager.h"
#include "InputEventsEnum.h"
#include "EventEnum.h"
#include "Camera.h"
#include "Tools/Map.h"
#include "IniReader.h"

#include <map>

class Fader;

class MainWorld : public World
{
public:
	MainWorld(bool& running);
	~MainWorld();

	void ProcessInput() override;
	void Update() override;
	void Draw() override;

private:
	//Fonction de creation principale du monde, appelee depuis le constructeur
	void CreateWorld();

	//Lance un Event a partir d'une touche attribuee
	void PostEventFromInput();

	//Initialise this->keyToEvent
	void InitKeyToEvent();

	//Initialise les tiles et les place pour former le sol (this->ground)
	void LoadGround();

	//Lit l'attribut this->scene a partir d'un fichier
	void LoadScene(int nb);
	
	//Cree des entites a partir d'un fichier binaire
	void LoadEntities(std::string bin_map_file_path);

	//Verifie chaque couple d'entites mobiles pour lancer l'Event de collision
	void DetectCollisions();

	/*Change l'attribut alpha du fader en fonction de la position
	du joueur par rapport aux bords de la scene, s'il en est suffisamment proche*/
	void CheckIfShouldFade();

protected:
	std::vector<std::unique_ptr<Entity>> entities;

	std::unique_ptr<Camera> camera;
	Entity* player;
	Fader* fader;
	std::unique_ptr<ControllerFactory> cf;
	std::unique_ptr<IniReader> iniReader;

	std::unique_ptr<std::map<InputEventEnum, EventEnum>> keyToEvent;
	std::unique_ptr<Texture> ground;

	std::unique_ptr<Map> scene;
};

