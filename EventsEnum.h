#pragma once

#include <SDL.h>

enum KeyEvent
{
	K_w = SDL_SCANCODE_W,
	K_s = SDL_SCANCODE_S,
	K_d = SDL_SCANCODE_D,
	K_a = SDL_SCANCODE_A,

	K_UP = SDL_SCANCODE_UP,
	K_DOWN = SDL_SCANCODE_DOWN,
	K_LEFT = SDL_SCANCODE_LEFT,
	K_RIGHT = SDL_SCANCODE_RIGHT,
};

enum GeneralEvent
{
	QUIT = SDL_QUIT,
};