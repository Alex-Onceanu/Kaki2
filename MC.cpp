#include "MC.h"
#include "InputEventsEnum.h"
#include "Input.h"

#include <SDL.h>

#include <sstream>
#include <array>
#include <string>
#include <memory>
#include <unordered_map>
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

	pos = { 200,200 };
	
	UpdateRectCoordinates();

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

	eventToFunction = std::make_unique<std::unordered_map<EventEnum, void(MC::*)()>>();

	for (int i = 0; i < events.size(); i++)
	{
		eventToFunction->insert({ events[i],functions[i] });
	}
}

void MC::MoveUp()
{
	std::cout << "Up !" << std::endl;
	shouldMirrorBlit = false;
	*currentAnimation = (*allAnimations)[INDEX_UP];
	speed = { 0,-SPEED_REF };
}

void MC::MoveDown()
{
	std::cout << "Down !" << std::endl;
	shouldMirrorBlit = false;
	*currentAnimation = (*allAnimations)[INDEX_DOWN];
	speed = { 0, SPEED_REF };
}

void MC::MoveLeft()
{
	std::cout << "Left !" << std::endl;
	shouldMirrorBlit = false;
	*currentAnimation = (*allAnimations)[INDEX_LEFT];
	speed = { -SPEED_REF,0 };
}

void MC::MoveRight()
{
	std::cout << "Right !" << std::endl;
	shouldMirrorBlit = true;
	*currentAnimation = (*allAnimations)[INDEX_LEFT];
	speed = { SPEED_REF,0 };
}

void MC::StopUp()
{
	speed.y = 0;
}

void MC::StopDown()
{
	speed.y = 0;
}

void MC::StopRight()
{
	speed.x = 0;
}

void MC::StopLeft()
{
	speed.x = 0;
}

void MC::UpdateRectCoordinates()
{
	rect.x = pos.x;
	rect.y = pos.y;
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

void MC::ProcessInput()
{

}

void MC::Update()
{
	if (++frameCount > 3600) frameCount = 0;

	if (frameCount % 12 == 0)
	{
		if (++animCount > 3) animCount = 0;
	}

	pos += speed;

	UpdateRectCoordinates();
}

void MC::Draw()
{
	if (shouldMirrorBlit)
	{
		Renderer::FullBlitMirrorHorizontal((*currentAnimation)[animCount].get(), rect);
	}
	else
	{
		Renderer::FullBlit((*currentAnimation)[animCount].get(), rect);
	}
}
