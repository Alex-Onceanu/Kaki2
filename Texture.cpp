
#include <SDL.h>

#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer, Surface* srcSurface)
{
	rendererTexture = SDL_CreateTextureFromSurface(renderer, *srcSurface->GetRendererSurface());
}

Texture::Texture(SDL_Renderer* renderer, SDL_Surface* srcSurface)
{
	rendererTexture = SDL_CreateTextureFromSurface(renderer, srcSurface);
}

Texture::~Texture()
{
	SDL_DestroyTexture(rendererTexture);
}

SDL_Texture* Texture::GetRendererTexture() const
{
	return rendererTexture;
}

void Texture::GetSize(int& w, int& h)
{
	SDL_QueryTexture(rendererTexture, NULL, NULL, &w, &h);
}