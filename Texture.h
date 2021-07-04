#pragma once


#include "Surface.h"

struct SDL_Texture;
struct SDL_Renderer;

class Texture
{
public:
	Texture(SDL_Renderer* renderer, Surface* srcSurface);
	Texture(SDL_Renderer* renderer, SDL_Surface* srcSurface);

	~Texture();

	SDL_Texture* GetRendererTexture() const;

	void GetSize(int& w, int& h);

private:
	SDL_Texture* rendererTexture;
};