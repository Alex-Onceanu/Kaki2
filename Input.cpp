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

	bool CheckEvent(GeneralInput gE)
	{
		auto e = InputEvent();
		while (SDL_PollEvent(e.GetRendererEvent()))
		{
			if (e.GetType() == static_cast<int>(gE))
			{
				return true;
			}
		}
		return false;
	}

	bool GetNextEvent(KeyInput* src)
	{
		auto e = InputEvent();
		bool res = SDL_PollEvent(e.GetRendererEvent());
		*src = static_cast<KeyInput>(e.GetRendererEvent()->key.keysym.scancode);
		return res;
	}

	
	bool CheckKeyPress(KeyInput k)
	{
		UpdateKeyboardState();
		return keyboard[static_cast<int>(k)];
	}
}
