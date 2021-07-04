#include "MC.h"
#include "InputEventsEnum.h"
#include "Input.h"

#include <SDL.h>

#include <sstream>
#include <array>
#include <string>
#include <memory>
#include <map>
#include <cassert>
#include <iostream>

MC::MC(TextureManager* tm_)
	:WorldEntity(tm_),
	Listener(),
	frameCount(0),
	animCount(0),
	speed({0})
{
	allAnimations = std::make_unique<std::vector<std::vector<std::shared_ptr<Texture>>>>();
	currentAnimation = std::make_unique<std::vector<shared_ptr<Texture>>>();
	
	LoadAllImages();

	*currentAnimation = (*allAnimations)[INDEX_DOWN];
	rect = Renderer::GetRect((*currentAnimation)[frameCount].get());
	//rect.w *= 2;
	//rect.h *= 2;

	pos = { 1800,1200 };
	
	InitEventToFunction();
}

MC::~MC()
{

}

void MC::InitEventToFunction()
{
	std::vector<EventEnum> events{
		EventEnum::MOVE_UP,
		EventEnum::MOVE_DOWN,
		EventEnum::MOVE_LEFT,
		EventEnum::MOVE_RIGHT,
	
		EventEnum::STOP_UP,
		EventEnum::STOP_DOWN,
		EventEnum::STOP_LEFT,
		EventEnum::STOP_RIGHT, };

	std::vector<void (MC::*)()> functions{
		&MC::MoveUp,
		&MC::MoveDown,
		&MC::MoveLeft,
		&MC::MoveRight,

		& MC::StopUp,
		& MC::StopDown,
		& MC::StopLeft,
		& MC::StopRight,
	};

	assert(events.size() == functions.size());

	eventToFunction = std::make_unique<std::map<EventEnum, void(MC::*)()>>();

	for (int i = 0; i < events.size(); i++)
	{
		eventToFunction->insert({ events[i],functions[i] });
	}
}

void MC::MoveUp()
{
	if (!move_direction.test(MOVING_UP))
		speed.y -= SPEED_REF;

	move_direction.set(MOVING_UP);
}

void MC::MoveDown()
{
	if(!move_direction.test(MOVING_DOWN))
		speed.y += SPEED_REF;

	move_direction.set(MOVING_DOWN);
}

void MC::MoveLeft()
{
	if (!move_direction.test(MOVING_LEFT))
		speed.x -= SPEED_REF;
	move_direction.set(MOVING_LEFT);
}

void MC::MoveRight()
{
	if (!move_direction.test(MOVING_RIGHT))
		speed.x += SPEED_REF;

	move_direction.set(MOVING_RIGHT);
}


void MC::StopUp()
{
	if (move_direction[MOVING_UP])
	{
		speed.y += SPEED_REF;
		move_direction[MOVING_UP] = 0;
	}
}

void MC::StopDown()
{
	if (move_direction[MOVING_DOWN])
	{
		speed.y -= SPEED_REF;
		move_direction[MOVING_DOWN] = 0;
	}
}

void MC::StopRight()
{
	if (move_direction[MOVING_RIGHT])
	{
		speed.x -= SPEED_REF;
		move_direction[MOVING_RIGHT] = 0;
	}
}

void MC::StopLeft()
{
	if (move_direction[MOVING_LEFT])
	{
		speed.x += SPEED_REF;
		move_direction[MOVING_LEFT] = 0;
	}
}

void MC::LoadAllImages()
{
	std::vector<std::string> tab{ "up", "down", "left" };

	for (int j = 0; j < tab.size(); j++)
	{
		std::vector<shared_ptr<Texture>> v{};
		allAnimations->push_back(v);
		bool go = true;
		for (int i = 1; go; i++)
		{
			try
			{
				std::ostringstream path;
				path << "./Assets/MC/" << tab[j] << "/" << i << ".png";
				(*allAnimations)[j].push_back(tm->GetTexture(path.str()));
			}
			catch (...)
			{
				go = false;
			}
		}
	}
}

void MC::ProcessCollision()
{
	return;
}

bool MC::CanProcess(Event* e)
{
	return (eventToFunction->find(e->type) != eventToFunction->end());
}

void MC::OnEvent(Event* e)
{
	auto funcPtr = eventToFunction->find(e->type);
	if (funcPtr != eventToFunction->end())
	{
		void (MC:: * f)() = funcPtr->second;
		(this->*f)();
	}
}

const Position* MC::GetPositionPtr()
{
	return &pos;
}

const Rect* MC::GetRectPtr()
{
	return &rect;
}

void MC::GetSizePtr(const int** w, const int** h)
{
	*w = &rect.w;
	*h = &rect.h;
}

void MC::UpdateAnimation()
{
	if (++frameCount > 3600) frameCount = 0;

	if (frameCount % 12 == 0)
	{
		if (++animCount > 3) animCount = 0;
	}

	if (speed.x > 0)
	{
		shouldMirrorBlit = true;
		*currentAnimation = (*allAnimations)[INDEX_LEFT];
	}
	else if (speed.x < 0)
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_LEFT];
	}
	else if (speed.y > 0)
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_DOWN];
	}
	else if (speed.y < 0)
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_UP];
	}
	else
	{
		animCount = 0;
	}
}

void MC::ProcessInput()
{

}

void MC::Update()
{
	UpdateAnimation();

	if (move_direction.any())
	{
		pos.x += (int)speed.x;
		pos.y += (int)speed.y;
	}
}

void MC::Draw(Position cameraPos)
{
	rect.x = pos.x - cameraPos.x;
	rect.y = pos.y - cameraPos.y;

	if (shouldMirrorBlit)
	{
		Renderer::FullBlitMirrorHorizontal((*currentAnimation)[animCount].get(), rect);
	}
	else
	{
		Renderer::FullBlit((*currentAnimation)[animCount].get(), rect);
	}
}
