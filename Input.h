#pragma once

#include <tuple>

#include "Position.h"
#include "InputEventsEnum.h"
#include "InputEvent.h"

namespace Input
{
	void UpdateKeyboardState();

	bool CheckEvent(InputEventEnum* i);
	bool CheckKeyPress(InputEventEnum k);
	bool GetNextInputEvent(InputEventEnum* src);

	//Renvoie les coordonnées x,y de la souris
	Position GetMousePos();

	//Renvoie l'etat des boutons gauche et droite, et les coordonnées de la souris
	std::pair<bool, bool> GetMouseState(int* x, int* y);
}