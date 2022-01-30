#include "pch.h"

#include "Font.h"

#include <SDL_ttf.h>

Font::Font(std::string path, int size)
{
	rendererFont = TTF_OpenFont(path.c_str(), size);
}

Font::~Font()
{
	TTF_CloseFont(rendererFont);
}

TTF_Font** Font::GetRendererFont()
{
	return &rendererFont;
}