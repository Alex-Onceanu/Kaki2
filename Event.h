#pragma once

#include "EventEnum.h"

#include <string>
#include <memory>

class Event
{
public:
	Event(EventEnum type_);

	EventEnum type;
};

class Listener
{
public:
	Listener();
	virtual bool CanProcess(Event* e) = 0;
	virtual void OnEvent(Event* e) = 0;
};