#include "Camera.h"
#include "Resolution.h"
#include "Utility.h"

Camera::Camera()
	:rect({ 0,0,RES_X,RES_Y })
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

void Camera::UpdatePosition(const Position& MCpos, const int& clampX, const int& clampY)
{
	SetPos(MCpos);
	Clamp(rect.x, 0, clampX);
	Clamp(rect.y, 0, clampY);
}
