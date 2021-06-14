
#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"

struct Position;

void Renderer::Init()
{
	try
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw("SDL could not be loaded.");
		}

		window = SDL_CreateWindow("Kaki 2",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			RES_X, RES_Y,
			NULL);

		/*
		auto icon = IMG_Load("./Assets/icon.png");
		if (!icon)
			throw (SDL_GetError());
		else
			SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
		*/

		if (window == NULL)
		{
			throw("Window could not be created.");
		}

		renderer = SDL_CreateRenderer(window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (renderer == NULL)
		{
			throw("Renderer could not be created.");
		}
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt");
		log << e << std::endl;
	}

}

void Renderer::Destroy()
{
	if (window)
		SDL_DestroyWindow(window);

	if (renderer)
		SDL_DestroyRenderer(renderer);

	SDL_Quit();
}

void Renderer::Flip()
{
	SDL_RenderPresent(renderer);
}

void Renderer::FullBlit(Texture* texture, Rect& rect)
{
	SDL_RenderCopy(renderer, 
		texture->GetRendererTexture(), 
		NULL, 
		(SDL_Rect *)&rect);
}

void Renderer::FullBlitMirror(Texture* texture, Rect& rect)
{
	SDL_RenderCopyEx(renderer,
		texture->GetRendererTexture(),
		NULL, 
		(SDL_Rect*)&rect,
		0, 
		nullptr, 
		SDL_FLIP_HORIZONTAL);
}

void Renderer::Blit(Texture* texture, Rect& srcRect, Rect& dstRect)
{
	SDL_RenderCopy(renderer,
		texture->GetRendererTexture(),
		(SDL_Rect *)&srcRect,
		(SDL_Rect *)&dstRect);
}

std::shared_ptr<Texture> Renderer::LoadImage(const std::string_view path)
{
	try {
		SDL_Surface* surf = IMG_Load(path.data());

		if (surf == NULL)
		{
			throw(std::string("Image could not be loaded at ") + path.data());
		}

		auto img = std::make_shared<Texture>(renderer, surf);
		SDL_FreeSurface(surf);

		return std::move(img);
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt");
		log << e << std::endl;
	}
	return nullptr;
}

Rect Renderer::GetRect(Texture* srcImg)
{
	int w, h;
	srcImg->GetSize(w, h);
	return { 0, 0, w, h };
}

void Renderer::ChangeRectSize(Rect& rect, const int newW, const int newH)
{
	rect.w = newW;
	rect.h = newH;
}

void Renderer::ChangeScaledRectSize(Rect& rect, const int newW)
{
	rect.h = (newW * rect.h) / rect.w;
	rect.w = newW;
}

void Renderer::DrawRect(Rect& rect, int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, (SDL_Rect*)&rect);
}

void Renderer::DrawFillRect(Rect& rect, int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, (SDL_Rect*)&rect);
}

void Renderer::DrawFillRectAlpha(Rect& rect, int r, int g, int b, int a)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, (SDL_Rect*)&rect);
}

void Renderer::Clear(int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}