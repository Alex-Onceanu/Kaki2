#pragma once


class Entity
{
public:
	virtual ~Entity() {};

	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

