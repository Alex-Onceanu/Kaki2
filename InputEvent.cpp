#include "pch.h"
#include "InputEvent.h"

#include <SDL.h>

#include <memory>

InputEvent::InputEvent()
{
	rendererEvent = std::make_unique<SDL_Event>();
}

SDL_Event* InputEvent::GetRendererEvent()
{
	return rendererEvent.get();
}

int InputEvent::GetType()
{
	return rendererEvent->type;
}