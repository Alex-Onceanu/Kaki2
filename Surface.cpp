#include <SDL.h>

#include "Surface.h"

Surface::Surface(SDL_Surface* srcSurface)
	:rendererSurface(srcSurface)
{

}

Surface::Surface(int width, int height)
{
	rendererSurface = SDL_CreateRGBSurface(NULL, width, height, 32, 0, 0, 0, 0);
}

Surface::~Surface()
{
	SDL_FreeSurface(rendererSurface);
}

SDL_Surface** Surface::GetRendererSurface()
{
	return &rendererSurface;
}

void Surface::GetSize(int& w, int& h)
{
	w = rendererSurface->w;
	h = rendererSurface->h;
}