#include "Input.h"
#include "Event.h"

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
		auto e = Event();
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
		if (keyboard[k] == 1) std::cout << "YEAA" << std::endl;
		return keyboard[k];
	}
	
	/*
	bool CheckKeyPress(KeyEvent k)
	{
		auto e = Event();

		while (SDL_PollEvent(e.GetRendererEvent()))
		{
			if (e.GetType() == SDL_KEYDOWN)
			{
				if (e.GetRendererEvent()->key.keysym.sym == k)
				{
					return true;
				}
			}
		}
		return false;
	}
	*/
}