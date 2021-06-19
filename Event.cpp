#include "Event.h"

#include "EventSystem.h"

Event::Event(EventEnum type_)
	:type(type_)
{
	
}


Listener::Listener()
{
	EventSystem::AddListener(this);
}

bool Listener::CanProcess(Event* e)
{
	return false;
}

void Listener::OnEvent(Event* e)
{
	return;
}