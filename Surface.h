#pragma once


struct SDL_Texture;
struct SDL_Surface;
struct SDL_Renderer;

class Surface
{
public:
	Surface(SDL_Surface* srcSurface);
	Surface(int width, int height);
	~Surface();

	SDL_Surface** GetRendererSurface();

	void GetSize(int& w, int& h);

private:
	SDL_Surface* rendererSurface;
};