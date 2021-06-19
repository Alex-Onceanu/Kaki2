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

	bool CheckEvent(InputEventEnum* i)
	{
		auto e = InputEvent();
		while (SDL_PollEvent(e.GetRendererEvent()))
		{
			if (e.GetType() == static_cast<int>(*i))
			{
				return true;
			}
		}
		return false;
	}

	bool GetNextInputEvent(InputEventEnum* src)
	{
		auto e = InputEvent();

		while (SDL_PollEvent(e.GetRendererEvent()))
		{
			if (e.GetType() == static_cast<int>(InputEventEnum::KEYDOWN))
			{
				*src = static_cast<InputEventEnum>(e.GetRendererEvent()->key.keysym.scancode);
				return true;
			}
			if (e.GetType() == static_cast<int>(InputEventEnum::QUIT))
			{
				*src = static_cast<InputEventEnum>(e.GetType());
				return true;
			}
		}
		return false;
	}
	
	bool CheckKeyPress(InputEventEnum k)
	{
		UpdateKeyboardState();
		return keyboard[static_cast<int>(k)];
	}
}
