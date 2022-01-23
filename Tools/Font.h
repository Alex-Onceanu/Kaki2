#pragma once

#include <string>

#include <SDL_ttf.h>

class Font
{
public:
	Font(std::string path, int size);
	~Font();

	TTF_Font** GetRendererFont();

private:
	TTF_Font* rendererFont;
};