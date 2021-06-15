#include "Event.h"

#include "EventSystem.h"

Event::Event(EventEnum type_)
	:type(type_)
{
	
}

void Event::Post()
{
	EventSystem::Post(this);
}

void Event::Launch()
{
	EventSystem::Launch(this);
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