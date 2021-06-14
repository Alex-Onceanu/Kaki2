#include "Input.h"
#include "InputEvent.h"

#include <SDL.h>

#include <iostream>
namespace Input
{
	auto keyboard = SDL_GetKeyboardState(NULL);

	void UpdateKeyboardState()
	{
		SDL_PumpEvents();
	}

	int GetMouseInput(int& x, int& y)
	{
		SDL_PumpEvents();
		return (int)SDL_GetMouseState(&x, &y);
	}

	bool CheckEvent(GeneralEvent gE)
	{
		auto e = InputEvent();
		while (SDL_PollEvent(e.GetRendererEvent()))
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
		UpdateKeyboardState();
		return keyboard[k];
	}
}
