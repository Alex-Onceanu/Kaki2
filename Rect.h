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

	template<typename T>
	Rect& operator*(const T& c)
	{
		Rect tmp = { int(x * c), int(y * c),int(w * c),int(h * c) };
		return tmp;
	}

};
