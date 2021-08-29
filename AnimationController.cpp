#include <memory>
#include <vector>
#include <sstream>

#include "AnimationController.h"
#include "EntityController.h"
#include "Entity.h"
#include "Renderer.h"
#include "Texture.h"
#include "TextureManager.h"


AnimationController::AnimationController(Entity* const o)
	: EntityController(o)
{
	allAnimations = std::make_unique<std::vector<std::vector<std::shared_ptr<Texture>>>>();
	currentAnimation = std::make_unique<std::vector<std::shared_ptr<Texture>>>();

	owner_pos = owner->GetPositionPtr();
	owner->GetSizePtr(&owner_w, &owner_h);
}

void AnimationController::SetPath(const std::string path)
{
	folderPath = path;
	LoadAllImages();

	*currentAnimation = (*allAnimations)[INDEX_DOWN];

	auto r = Renderer::GetRect((*currentAnimation)[0].get());

	/*owner_w = r.w;
	*owner_h = r.h;*/
	*owner_w = 50;
	*owner_h = (*owner_w * r.h) / r.w;
}

void AnimationController::LoadInitialData(std::map<std::string, std::string>& ini)
{
	SetPath(ini["path"]);
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
				(*allAnimations)[j].push_back(TextureManager::GetTexture(path.str()));
			}
			catch (...)
			{
				break;
			}
		}
	}
}

bool AnimationController::CanProcess(Event* e)
{
	return((e->type == EventEnum::COLLISION)
		and (static_cast<Entity*>(e->args) == owner));
}

void AnimationController::OnEvent(Event* e)
{
	animCount = 0;
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

std::unique_ptr<EntityController> AnimationControllerCreator::operator()(Entity* owner)
{
	return std::move(std::make_unique<AnimationController>(owner));
}