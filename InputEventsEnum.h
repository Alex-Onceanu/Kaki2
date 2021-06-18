#pragma once

#include <SDL.h>

enum class KeyInput
{
	W = SDL_SCANCODE_W,
	S = SDL_SCANCODE_S,
	D = SDL_SCANCODE_D,
	A = SDL_SCANCODE_A,

	UP = SDL_SCANCODE_UP,
	DOWN = SDL_SCANCODE_DOWN,
	LEFT = SDL_SCANCODE_LEFT,
	RIGHT = SDL_SCANCODE_RIGHT,
};

enum class GeneralInput
{
	QUIT = SDL_QUIT,
};