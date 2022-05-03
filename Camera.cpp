#include "Camera.h"
#include "Resolution.h"
#include "Utility.h"

Camera::Camera(const Position* mcPos, const int* mcW, const int* mcH, const int __map_W, const int __map_H)
	: playerPos(mcPos),
	playerW(mcW),
	playerH(mcH),
	map_W(__map_W),
	map_H(__map_H)
{

}

Camera::~Camera()
{

}

Position Camera::GetPos()
{
	return { rect.x,rect.y };
}

void Camera::SetPos(const Position& p)
{
	rect.x = p.x;
	rect.y = p.y;
}

void Camera::SetPos(const int& x, const int& y)
{
	rect.x = x;
	rect.y = y;
}

void Camera::AddPos(const Position& p)
{
	rect.x += p.x;
	rect.y += p.y;
}

void Camera::AddPos(const int& x, const int& y)
{
	rect.x += x;
	rect.y += y;
}

void Camera::GetSize(int& w, int& h)
{
	w = rect.w;
	h = rect.h;
}

void Camera::UpdatePosition(const int& clampX, const int& clampY)
{
	Position pr{ *playerPos };
	Position add{
		RES_X / 2 - *playerW / 2,
		RES_Y / 2 - *playerH / 2 };

	pr -= add;

	SetPos(pr);
	bool is_stopped_x = Clamp(rect.x, 0, clampX);
	bool is_stopped_y = Clamp(rect.y, 0, clampY);
	this->is_stopped = is_stopped_x or is_stopped_y;

	//if (clampX < 0) SetPos(int(-RES_X / 2 + map_W / 2), rect.y);
	//if (clampY < 0) SetPos(rect.x, int(-RES_Y / 2 + map_H / 2));
}
