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
				l->Process(ev);
			}
		}
	}

	void Post(Event* ev)
	{
		postEvents.push_back(ev);
	}

	void Update()
	{
		for (auto&& pe : postEvents)
		{
			Launch(pe);
		}
		postEvents.clear();
	}
}