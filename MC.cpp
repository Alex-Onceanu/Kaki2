#include "MC.h"

#include <sstream>
#include <array>
#include <string>
#include <memory>

MC::MC(TextureManager* tm_)
	:WorldEntity(tm_),
	frameCount(0)
{
	allAnimations = std::make_unique<std::vector<std::vector<std::shared_ptr<Texture>>>>();
	currentAnimation = std::make_unique<std::vector<shared_ptr<Texture>>>();
	
	LoadAllImages();

	*currentAnimation = (*allAnimations)[INDEX_DOWN];
	rect = Renderer::GetRect((*currentAnimation)[frameCount].get());

	pos = { 200,200 };
	
	UpdateRectCoordinates();
	comppt = 0;
}

MC::~MC()
{

}

void MC::UpdateRectCoordinates()
{
	rect.x = pos.x;
	rect.y = pos.y;
}

void MC::LoadAllImages()
{
	std::vector<std::string> tab{ "up", "down", "right", "left" };

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

void MC::ProcessCollision()
{
	return;
}

void MC::ProcessInput()
{
	
}

void MC::Update()
{
	if (++frameCount > 3) frameCount = 0;
	
	UpdateRectCoordinates();
}

void MC::Draw()
{
	Renderer::FullBlit((*currentAnimation)[frameCount].get(), rect);
}
