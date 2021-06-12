#pragma once

#include <SDL.h>

enum KeyEvent
{
	K_UP = SDLK_UP,
	K_DOWN = SDLK_DOWN,
	K_LEFT = SDLK_LEFT,
	K_RIGHT = SDLK_RIGHT,
};

enum GeneralEvent
{
	QUIT = SDL_QUIT,
};