#pragma once

#include "World.h"
#include "TextureManager.h"
#include "InputEventsEnum.h"
#include "EventEnum.h"
#include "Camera.h"
#include "WorldEntity.h"
#include "Tools/Map.h"

#include <map>

class MainWorld : public World
{
public:
	MainWorld(bool& running);
	~MainWorld();

	void ProcessInput() override;
	void Update() override;
	void Draw() override;

private:
	void CreateWorld();
	void PostEventFromInput();
	void InitKeyToEvent();
	void LoadGround();
	void LoadScene(int nb);

protected:
	std::vector<std::unique_ptr<WorldEntity>> entities;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<TextureManager> tm;

	std::unique_ptr<std::map<InputEventEnum, EventEnum>> keyToEvent;
	std::unique_ptr<Texture> ground;

	std::unique_ptr<Map> scene;
};

