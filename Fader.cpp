#include "Fader.h"

#include "Resolution.h"
#include "Renderer.h"

#include <iostream>

Fader::Fader()
	: Entity()
{
	rect_fade = Rect({ 0,0,RES_X,RES_Y });
}

void Fader::SetColor(int r_, int g_, int b_)
{
	r = r_; g = g_; b = b_;
}

void Fader::SetAlpha(int alpha_)
{
	alpha = alpha_;
}

int Fader::GetAlpha()
{
	return alpha;
}

void Fader::Draw(Position cameraPos)
{
	if (alpha != 0)
	{
		Renderer::DrawFillRectAlpha(rect_fade, r, g, b, alpha);
	}
}