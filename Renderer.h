#pragma once

#include <fstream>
#include <string>
#include <memory>
#include <vector>



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
	void Init(std::string windowName);
	void Destroy();

	SDL_Renderer** GetRenderer();

	//Actualise l'écran
	void Flip();


	//Affiche une texture (entiere) sur une partie de l'écran en fonction du rect
	void FullBlit(Texture* texture, Rect& rect);
	//FullBlit mais retourne horizontalement la Texture
	void FullBlitMirrorHorizontal(Texture* texture, Rect& rect);
	//Affiche une partie d'une Texture en fonction de srcRect sur une partie de l'écran en fonction de dstRect
	void Blit(Texture* texture, Rect& srcRect, Rect& dstRect);
	

	//Charge une image dans une Texture
	std::shared_ptr<Texture> LoadImage(const std::string_view path);
	//Charge une image dans une Surface
	std::shared_ptr<Surface> SurfaceLoadImage(const std::string_view path);


	//Renvoie un Rect collé aux bords d'une Texture (collision box)
	Rect GetRect(Texture*);
	//Choisit des nouvelles dimensions pour un Rect
	void ChangeRectSize(Rect& rect, const int newW, const int newH);
	//Choisit des nouvelles dimensions pour un Rect en gardant la proportion initiale, indiquer juste le nouveau Width
	void ChangeScaledRectSize(Rect& rect, const int newW);

	//Colorie entierement une surface
	void ClearSurface(Surface* dst, int r, int g, int b);
	//Colle un Rect sur une surface
	void DrawFilledRectOnSurface(Rect& src, Surface* dst, int r, int g, int b);
	//Colle un nombre count de Rects sur une surface
	void DrawMoreFilledRectsOnSurface(const Rect* src, int count, Surface* dst, int r, int g, int b);

	//Dessine tous les points du vecteur de leur position
	void DrawPoints(std::vector<Position> pts, int r, int g, int b, int a);
	//Dessine des lignes *connectees* a partir de leurs positions
	void DrawLines(std::vector<Position>, int r, int g, int b, int a);
	//Dessine les bords d'un Rect
	void DrawRect(Rect& rect, int r, int g, int b);
	//Dessine le Rect entier rempli colorié
	void DrawFillRect(Rect& rect, int r, int g, int b);
	//Dessine le Rect entier rempli colorié + transparent en fonction de a (alpha 0 = invisible, alpha 255 = opaque)
	void DrawFillRectAlpha(Rect& rect, int r, int g, int b, int a);


	//Privilegier plutot CopySurfaceScaled
	void CopySurface(Surface* src, Rect* srcRect, Surface* dst, Rect* dstRect);
	//Copie une Surface src dans dst, on peut choisir quelle partie copier et où coller la surface avec srcRect et dstRect
	void CopySurfaceScaled(Surface* src, Rect* srcRect, Surface* dst, Rect* dstRect);

	void SetScale(double s);

	//Efface l'écran avec une couleur RGB
	void Clear(int r, int g, int b);

	void ShowCursor(bool yes);
};

