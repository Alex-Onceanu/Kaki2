#pragma once

#include "InputEventsEnum.h"
#include "InputEvent.h"

namespace Input
{
	void UpdateKeyboardState();

	bool CheckEvent(GeneralInput gE);
	bool CheckKeyPress(KeyInput k);
	bool GetNextEvent(KeyInput* src);
}