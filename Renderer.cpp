#include "Tools/pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Resolution.h"
#include "Renderer.h"
#include "Position.h"
#include "Tools/Font.h"


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
		std::ofstream log("log.txt", std::ios_base::app);
		log << e << std::endl;
	}

}

void Renderer::Init(std::string windowName)
{
	try
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw("SDL could not be loaded.");
		}

		window = SDL_CreateWindow(windowName.c_str(),
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

		if (TTF_Init() != 0)
		{
			throw("TTF could not be started.");
		}
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
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

SDL_Renderer** Renderer::GetRenderer()
{
	return &renderer;
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

void Renderer::FullBlitMirrorHorizontal(Texture* texture, Rect& rect)
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
		std::ofstream log("log.txt", std::ios_base::app);
		log << e << std::endl;
	}
	return nullptr;
}

std::shared_ptr<Surface> Renderer::SurfaceLoadImage(const std::string_view path)
{
	try {
		SDL_Surface* surf = IMG_Load(path.data());

		if (surf == NULL)
		{
			throw(std::string("Image could not be loaded at ") + path.data());
		}

		auto img = std::make_shared<Surface>(surf);
		
		return std::move(img);
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << e << std::endl;
	}
	return nullptr;
}

std::shared_ptr<Texture> Renderer::LoadTextTexture(const std::string text, Font* f, int r, int g, int b, int size)
{
	try {
		SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(*(f->GetRendererFont()), text.c_str(), SDL_Color(r, g, b, 255), size);

		if (surf == NULL)
		{
			throw(text + std::string(" could not be loaded."));
		}

		std::shared_ptr<Texture> img = std::make_shared<Texture>(renderer, surf);
		SDL_FreeSurface(surf);

		return std::move(img);
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
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

void Renderer::ClearSurface(Surface* dst, int r, int g, int b)
{
	SDL_FillRect(*dst->GetRendererSurface(), NULL, SDL_MapRGB((*dst->GetRendererSurface())->format, r, g, b));
}

void Renderer::DrawFilledRectOnSurface(Rect& src, Surface* dst, int r, int g, int b)
{
	SDL_FillRect(*dst->GetRendererSurface(), (SDL_Rect*)&src, SDL_MapRGB((*dst->GetRendererSurface())->format,r,g,b));
}

void Renderer::DrawMoreFilledRectsOnSurface(const Rect* src, int count, Surface* dst, int r, int g, int b)
{
	SDL_FillRect(*dst->GetRendererSurface(), (SDL_Rect*)src, SDL_MapRGB((*dst->GetRendererSurface())->format, r, g, b));
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

void Renderer::CopySurface(Surface* src, Rect* srcRect, Surface* dst, Rect* dstRect)
{
	SDL_BlitSurface(*src->GetRendererSurface(), (SDL_Rect*)srcRect,
					*dst->GetRendererSurface(), (SDL_Rect*)dstRect);
}

void Renderer::CopySurfaceScaled(Surface* src, Rect* srcRect, Surface* dst, Rect* dstRect)
{
	SDL_BlitScaled(*src->GetRendererSurface(), (SDL_Rect*)srcRect,
		*dst->GetRendererSurface(), (SDL_Rect*)dstRect);
}

void Renderer::Clear(int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void Renderer::DrawPoints(std::vector<Position> pts, int r, int g, int b, int a)
{
	Uint8 oldr, oldg, oldb, olda;
	SDL_GetRenderDrawColor(renderer, &oldr, &oldg, &oldb, &olda);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawPoints(renderer, reinterpret_cast<SDL_Point*>(&pts[0]), int(pts.size()));
	SDL_SetRenderDrawColor(renderer, oldr, oldg, oldb, olda);
}

void Renderer::DrawLines(std::vector<Position> pts, int r, int g, int b, int a)
{
	Uint8 oldr, oldg, oldb, olda;
	SDL_GetRenderDrawColor(renderer, &oldr, &oldg, &oldb, &olda);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawLines(renderer, reinterpret_cast<SDL_Point *>(&pts[0]), int(pts.size()));
	SDL_SetRenderDrawColor(renderer, oldr, oldg, oldb, olda);
}

void Renderer::SetScale(double s)
{
	SDL_RenderSetScale(renderer, float(s), float(s));
}

void Renderer::ShowCursor(bool yes)
{
	SDL_ShowCursor(int(yes));
}