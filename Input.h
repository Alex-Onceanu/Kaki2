#pragma once

#include "EventsEnum.h"

namespace Input
{
	const char* GetKeyboardInput();

	int GetMouseInput(int& x, int& y);

	bool CheckEvent(GeneralEvent e);
	bool CheckKeyPress(KeyEvent k);

}