#pragma once

#include <string>
#include "EventEnum.h"

class Event
{
public:
	Event(EventEnum type_);
	void Post();
	void Launch();

	EventEnum type;
};

class Listener
{
public:
	Listener();
	virtual bool CanProcess(Event* e) = 0;
	virtual void OnEvent(Event* e) = 0;
};