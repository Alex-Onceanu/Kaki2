#pragma once

#include "Rect.h"
#include "Position.h"

class Camera
{
public:
	Camera(const Position* mcPos, const int* mcW, const int* mcH);
	~Camera();

	void UpdatePosition(const int& clampX, const int& clampY);

	Position GetPos();
	void SetPos(const Position& p);
	void SetPos(const int& x, const int& y);
	void AddPos(const Position& p);
	void AddPos(const int& x, const int& y);

	void GetSize(int& w, int& h);
	bool IsStopped() { return is_stopped; };

protected:
	Rect rect;

	const Position* playerPos;
	const int* playerW;
	const int* playerH;
	bool is_stopped;
};

