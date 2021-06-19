#pragma once

#include "Entity.h"
#include "Event.h"

#include <memory>
#include <vector>

namespace EventSystem
{
	static std::vector<Listener*> listeners;
	static std::vector <Event> postEvents;

	void AddListener(Listener* l);
	void Launch(Event* ev);
	void Post(Event ev);
	void Update();
};

