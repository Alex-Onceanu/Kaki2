#pragma once
#include "Entity.h"

struct Rect;

class Fader : public Entity
{
public:
	Fader();

	void Draw(Position cameraPos) override;

	void SetColor(int r_, int g_, int b_);
	void SetAlpha(int alpha_);
	int GetAlpha();

private:
	int r = 0; int g = 0; int b = 0;
	int alpha = 0;

	Rect rect_fade;
};

