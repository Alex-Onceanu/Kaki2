#include "Event.h"

#include <SDL.h>

#include <memory>

Event::Event()
{
	rendererEvent = std::make_unique<SDL_Event>();
	EventType = rendererEvent->type;
}

SDL_Event* Event::GetRendererEvent()
{
	return rendererEvent.get();
}