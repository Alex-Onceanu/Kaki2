#include "Tools/pch.h"
#include "Input.h"
#include "InputEvent.h"
#include "Position.h"

#include <SDL.h>

#include <vector>
#include <iostream>
#include <fstream>


namespace Input
{
	auto keyboard = SDL_GetKeyboardState(NULL);

	void UpdateKeyboardState()
	{
		SDL_PumpEvents();
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
			if (e.GetType() == static_cast<int>(InputEventEnum::KEYUP))
			{
				*src = static_cast<InputEventEnum>(e.GetRendererEvent()->key.keysym.scancode - IS_KEY_UP_ENUM);
				return true;
			}
			if (e.GetType() == static_cast<int>(InputEventEnum::SCROLL))
			{
				if (e.GetRendererEvent()->wheel.y > 0)
				{
					*src = InputEventEnum::SCROLL_UP;
					return true;
				}
				if (e.GetRendererEvent()->wheel.y < 0)
				{
					*src = InputEventEnum::SCROLL_DOWN;
					return true;
				}
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

std::pair<bool, bool> Input::GetMouseState(int* x, int* y)
{
	auto e = SDL_GetMouseState(x, y);
	bool gauche = (e & SDL_BUTTON_LMASK) != 0;
	bool droite = (e & SDL_BUTTON_RMASK) != 0;
	return std::make_pair(gauche, droite);
}

Position Input::GetMousePos()
{
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	return Position({ x, y });
}