#pragma once

#include <fstream>
#include <string>
#include <memory>



#include "Rect.h"
#include "Texture.h"
#include "Surface.h"


struct SDL_Window;
struct SDL_Texture;
struct SDL_Surface;
struct SDL_Renderer;

namespace Renderer
{

	static SDL_Window* window;
	static SDL_Renderer* renderer;

	void Init();
	void Destroy();

	SDL_Renderer** GetRenderer();
	void Flip();

	void FullBlit(Texture* texture, Rect& rect);
	void FullBlitMirrorHorizontal(Texture* texture, Rect& rect);
	void Blit(Texture* texture, Rect& srcRect, Rect& dstRect);
	
	std::shared_ptr<Texture> LoadImage(const std::string_view path);
	std::shared_ptr<Surface> SurfaceLoadImage(const std::string_view path);
	Rect GetRect(Texture*);
	void ChangeRectSize(Rect& rect, const int newW, const int newH);
	void ChangeScaledRectSize(Rect& rect, const int newW);
	void DrawRect(Rect& rect, int r, int g, int b);
	void DrawFillRect(Rect& rect, int r, int g, int b);
	void DrawFillRectAlpha(Rect& rect, int r, int g, int b, int a);
	void CopySurface(Surface* src, Rect* srcRect, Surface* dst, Rect* dstRect);

	void Clear(int r, int g, int b);

};

