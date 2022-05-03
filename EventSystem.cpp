#include "Tools/pch.h"
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
			if (l)
			{//??????????????????????????????
				//Des fois l est juste inaccessibile apres avoir changé de map
				// pourtant listeners est quand meme marqué comme vide
				if (l->CanProcess(ev))
				{
					l->OnEvent(ev);
				}
			}
		}
	}

	void Post(Event ev)
	{
		postEvents.push_back(ev);
	}

	void Update()
	{
		for (auto&& pe : postEvents)
		{
			Launch(&pe);
		}
		postEvents.clear();
	}

	void Clean()
	{
		listeners.resize(4);
	}
}