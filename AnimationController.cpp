#include <memory>
#include <vector>
#include <sstream>

#include "AnimationController.h"
#include "Entity.h"
#include "Renderer.h"
#include "Texture.h"

AnimationController::AnimationController(Entity* const o, TextureManager* tm_, const std::string folderPath_)
	: EntityController(o)
	, folderPath(folderPath_)
	, tm(tm_)
{
	allAnimations = std::make_unique<std::vector<std::vector<std::shared_ptr<Texture>>>>();
	currentAnimation = std::make_unique<std::vector<std::shared_ptr<Texture>>>();

	LoadAllImages();

	*currentAnimation = (*allAnimations)[INDEX_DOWN];

	owner_pos = owner->GetPositionPtr();
	owner->GetSizePtr(&owner_w, &owner_h);

	auto r = Renderer::GetRect((*currentAnimation)[0].get());

	/*owner_w = r.w;
	*owner_h = r.h;*/
	*owner_w = 50;
	*owner_h = (*owner_w * r.h) / r.w;
}

void AnimationController::LoadAllImages()
{
	std::vector<std::string> tab{ "up", "down", "left" };

	for (int j = 0; j < tab.size(); j++)
	{
		std::vector<std::shared_ptr<Texture>> v{};
		allAnimations->push_back(v);
		for (int i = 1; true; i++)
		{
			try
			{
				std::ostringstream path;
				path << folderPath << "/" << tab[j] << "/" << i << ".png";
				(*allAnimations)[j].push_back(tm->GetTexture(path.str()));
			}
			catch (...)
			{
				break;
			}
		}
	}
}

void AnimationController::Update()
{
	if (++frameCount > 3600) frameCount = 0;

	if (frameCount % 11 == 0)
	{
		if (++animCount > 3) animCount = 0;
	}

	Position speed = *owner_pos - oldPosition;
	oldPosition = *owner_pos;

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

void AnimationController::Draw(const Position &cameraPos)
{
	Rect rect = { owner_pos->x - cameraPos.x, owner_pos->y - cameraPos.y, *owner_w, *owner_h };
	if (shouldMirrorBlit)
	{
		Renderer::FullBlitMirrorHorizontal((*currentAnimation)[animCount].get(), rect);
	}
	else
	{
		Renderer::FullBlit((*currentAnimation)[animCount].get(), rect);
	}
}