#include "Input.h"
#include "Event.h"

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

	bool CheckEvent(GeneralEvent gE)
	{
		auto e = Event();
		while (SDL_PollEvent(e.GetRendererEvent()) != 0)
		{
			if (e.GetType() == gE)
			{
				return true;
			}
		}
		return false;
	}

	bool CheckKeyPress(KeyEvent k)
	{
		auto i = GetKeyboardInput();
		return i[k];
	}
}