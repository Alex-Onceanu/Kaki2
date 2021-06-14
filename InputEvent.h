#pragma once

#include <memory>

union SDL_Event;

class InputEvent
{
public:
	InputEvent();
	SDL_Event* GetRendererEvent();
	int GetType();

private:
	std::unique_ptr<SDL_Event> rendererEvent;
};

