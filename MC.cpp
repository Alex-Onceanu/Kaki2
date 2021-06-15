#include "MC.h"
#include "InputEventsEnum.h"
#include "Input.h"

#include <sstream>
#include <array>
#include <string>
#include <memory>
#include <SDL.h>

MC::MC(TextureManager* tm_)
	:WorldEntity(tm_),
	Listener(),
	frameCount(0),
	animCount(0),
	speed(5)
{
	allAnimations = std::make_unique<std::vector<std::vector<std::shared_ptr<Texture>>>>();
	currentAnimation = std::make_unique<std::vector<shared_ptr<Texture>>>();
	
	LoadAllImages();

	*currentAnimation = (*allAnimations)[INDEX_DOWN];
	rect = Renderer::GetRect((*currentAnimation)[frameCount].get());

	pos = { 200,200 };
	
	UpdateRectCoordinates();
}

MC::~MC()
{

}

void MC::UpdateRectCoordinates()
{
	rect.x = pos.x;
	rect.y = pos.y;
}

#if 0
void MC::LoadAllImages()
{
	std::vector<std::string> tab{ "up", "down", "left" };

	for (int j = 0; j < tab.size(); j++)
	{
		std::vector<shared_ptr<Texture>> v{};
		allAnimations->push_back(v);
		for (int i = 1; i < 5; i++)
		{
			std::ostringstream path;
			path << "./Assets/MC/"<< tab[j] <<"/" << i << ".png";
			(*allAnimations)[j].push_back(tm->GetTexture(path.str()));
		}
	}
}
#endif

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

#if 0
void MC::ProcessInput()
{
	if (Input::CheckKeyPress(K_DOWN) || Input::CheckKeyPress(K_s))
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_DOWN];
		pos.y += speed;
	}
	else if (Input::CheckKeyPress(K_UP) || Input::CheckKeyPress(K_w))
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_UP];
		pos.y -= speed;
	}
	else if (Input::CheckKeyPress(K_LEFT) || Input::CheckKeyPress(K_a))
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_LEFT];
		pos.x -= speed;
	}
	else if (Input::CheckKeyPress(K_RIGHT) || Input::CheckKeyPress(K_d))
	{
		shouldMirrorBlit = true;
		*currentAnimation = (*allAnimations)[INDEX_LEFT];
		pos.x += speed;
	}

}

#endif

bool MC::CanProcess(Event* e)
{
	return (e->type == EventEnum::MOVE_UP);
}

void MC::OnEvent(Event* e)
{
	if (e->type == EventEnum::MOVE_UP)
	{
		shouldMirrorBlit = false;
		*currentAnimation = (*allAnimations)[INDEX_UP];
		pos.y -= speed;
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
