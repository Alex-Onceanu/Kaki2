#pragma once

#include "EventsEnum.h"

namespace Input
{
	void UpdateKeyboardState();

	bool CheckEvent(GeneralEvent gE);
	bool CheckKeyPress(KeyEvent k);

}