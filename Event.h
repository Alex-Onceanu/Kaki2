#pragma once

#include <memory>

union SDL_Event;

class Event
{
public:
	Event();
	SDL_Event* GetRendererEvent();
	int GetType();

private:
	std::unique_ptr<SDL_Event> rendererEvent;
};

