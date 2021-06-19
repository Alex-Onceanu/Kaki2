#include "EventSystem.h"

namespace EventSystem
{
	void AddListener(Listener* l)
	{
		listeners.push_back(l);
	}

	void Launch(Event* ev)
	{
		for (auto&& l : listeners)
		{
			if (l->CanProcess(ev))
			{
				l->OnEvent(ev);
			}
		}
	}

	void Post(std::unique_ptr<Event> ev)
	{
		postEvents.push_back(std::move(ev));
	}

	void Update()
	{
		for (auto&& pe : postEvents)
		{
			Launch(pe.get());
		}
		postEvents.clear();
	}
}