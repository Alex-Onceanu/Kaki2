#pragma once

#include "InputEventsEnum.h"
#include "InputEvent.h"

namespace Input
{
	void UpdateKeyboardState();

	bool CheckEvent(InputEventEnum* i);
	bool CheckKeyPress(InputEventEnum k);
	bool GetNextInputEvent(InputEventEnum* src);
}