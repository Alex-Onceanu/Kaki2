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

	Position GetPosition() { return position; };
	void SetPosition(Position p) { position = p; };
	Position* GetPositionPtr() { return &position; };

	Rect* GetRectPtr() { return &rect; };
	void GetSizePtr(int** w, int** h)
	{
		*w = &rect.w;
		*h = &rect.h;
	};

	void GetSizePtr(const int** w, const int** h)
	{
		*w = &rect.w;
		*h = &rect.h;
	};

	bool IsSolid() { return solid; };
	void SetSolid(bool solid_) { solid = solid_; };
	bool IsMovible() { return movible; };
	void SetMovible(bool movible_) { movible = movible_; };
	 
protected:

	bool solid;
	bool movible;

	Position position;
	Rect rect;

	std::vector<std::unique_ptr<EntityController>> controllers = {};
};