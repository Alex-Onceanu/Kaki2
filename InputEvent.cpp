#include "InputEvent.h"

#include <SDL.h>

#include <memory>

Event::Event()
{
	rendererEvent = std::make_unique<SDL_Event>();
}

SDL_Event* Event::GetRendererEvent()
{
	return rendererEvent.get();
}

int Event::GetType()
{
	return rendererEvent->type;
}