#pragma once

#include "Rect.h"
#include "Position.h"
#include "ControllerFactory.h"

#include <vector>
#include <memory>

class InitialData;

class Entity
{
public:
	Entity();
	void SetControllers(std::vector<std::unique_ptr<EntityController>>& args);
	virtual ~Entity() {};

	virtual void LoadInitialData(ControllerFactory* cf, InitialData& ini);
	
	virtual void ProcessInput();
	virtual void Update();
	virtual void Draw(Position cameraPos);

	Position GetPosition();
	void SetPosition(Position p) { position = p; };
	Position* GetPositionPtr();

	Rect* GetRectPtr() { return &rect; };
	void GetSizePtr(int** w, int** h);
	void GetSizePtr(const int** w, const int** h);
	 
protected:
	Position position;
	Rect rect;

	std::vector<std::unique_ptr<EntityController>> controllers = {};
};