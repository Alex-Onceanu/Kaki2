#pragma once

#include <vector>
#include <memory>

#include "Position.h"
#include "EntityController.h"
#include "Rect.h"


class Entity
{
public:
	Entity()
	{
		position = { 0,0 };
		rect = { 0,0,0,0 };
	};

	Entity(std::vector<std::unique_ptr<EntityController>>& args)
	{
		position = { 0,0 };
		rect = { 0,0,0,0 };
		controllers = std::move(args);
	};

	virtual ~Entity() {};

	void InitControllers(std::vector<std::unique_ptr<EntityController>>& args)
	{
		controllers = std::move(args);
	};

	virtual void ProcessInput()
	{
		for (auto&& c : controllers)
		{
			c->ProcessInput();
		}
	};

	virtual void Update()
	{
		for (auto&& c : controllers)
		{
			c->Update();
		}
	};

	virtual void Draw(Position cameraPos)
	{
		for (auto&& c : controllers)
		{
			c->Draw(cameraPos);
		}
	};

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