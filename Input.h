#pragma once

#include "InputEventsEnum.h"

namespace Input
{
	void UpdateKeyboardState();

	bool CheckEvent(GeneralEvent gE);
	bool CheckKeyPress(KeyEvent k);

}