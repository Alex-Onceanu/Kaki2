#pragma once

#include "Rect.h"
#include "Position.h"

class Camera
{
public:
	Camera();
	~Camera();

	void UpdatePosition(const Position& MCpos, const int& clampX, const int& clampY);

	Position GetPos();
	void SetPos(const Position& p);
	void SetPos(const int& x, const int& y);
	void AddPos(const Position& p);
	void AddPos(const int& x, const int& y);

	void GetSize(int& w, int& h);

protected:
	Rect rect;
};

