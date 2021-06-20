#pragma once

struct Position
{
	int x, y;
};

static Position operator + (Position a, Position b)
{
	return { a.x + b.x, a.y + b.y };
}

static void operator += (Position& a, Position& b)
{
	a = a + b;
}