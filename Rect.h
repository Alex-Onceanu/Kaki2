#pragma once
#include "Position.h"

struct Rect
{
	int x, y;
	int w, h;

	Rect& operator= (const Position& b)
	{
		x = b.x;
		y = b.y;
		return *this;
	}
};
