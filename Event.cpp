#include "Event.h"

#include "EventSystem.h"

Event::Event(EventEnum type_, void* args_)
	: type(type_)
	, args(args_)
{
	
}

Event::Event(EventEnum type_)
	: type(type_)
	, args(nullptr)
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