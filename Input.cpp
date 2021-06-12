#include "Input.h"

#include <SDL.h>


namespace Input
{
	const char* GetKeyboardInput()
	{
		SDL_PumpEvents();
		return (char*)SDL_GetKeyboardState(NULL);
	}

	int GetMouseInput(int& x, int& y)
	{
		SDL_PumpEvents();
		return (int)SDL_GetMouseState(&x, &y);
	}

	bool CheckEvent()
	{

	}

	bool CheckKeyPress()
	{

	}
}