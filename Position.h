#pragma once

struct Position
{
	int x, y;
};

Position operator + (Position a, Position b)
{
	return { a.x + b.x, a.y + b.y };
}