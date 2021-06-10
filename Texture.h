#pragma once


struct SDL_Texture;
struct SDL_Surface;
struct SDL_Renderer;

class Texture
{
public:
	Texture(SDL_Renderer* renderer, SDL_Surface* srcSurface);

	~Texture();

	SDL_Texture* GetRendererTexture() const;

	void GetSize(int& w, int& h);

private:
	SDL_Texture* rendererTexture;
};