#include "ObstacleAnimationController.h"
#include "Entity.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Rect.h"

#include <sstream>


ObstacleAnimationController::ObstacleAnimationController(Entity* const o)
	: EntityController(o)
{
	frames = std::make_unique<std::vector<std::shared_ptr<Texture>>>();
	owner_pos = owner->GetPositionPtr();
	owner->GetSizePtr(&owner_w, &owner_h);
}

void ObstacleAnimationController::LoadInitialData(std::map<std::string, std::string>& ini)
{
	SetPath(ini["path"]);

	*owner_w = StrToInt(ini["width"]);
	*owner_h = StrToInt(ini["height"]);
}

void ObstacleAnimationController::SetPath(const std::string path)
{
	for (int i = 1; true; i++)
	{
		try
		{
			std::ostringstream p;
			p << path << "/" << i << ".png";
			frames->push_back(TextureManager::GetTexture(p.str()));
		}
		catch (...)
		{
			break;
		}
	}
}


void ObstacleAnimationController::Update()
{
	if (++frameCount > 3600) frameCount = 0;

	if (frameCount % 11 == 0)
	{
		animCount = (animCount + 1) % int(frames->size());
	}
}

void ObstacleAnimationController::Draw(const Position& cameraPos)
{
	Rect rect = { owner_pos->x - cameraPos.x, owner_pos->y - cameraPos.y, *owner_w, *owner_h };
	Renderer::FullBlit((*frames)[animCount].get(), rect);
}


std::unique_ptr<EntityController> ObstacleAnimationControllerCreator::operator()(Entity* owner)
{
	return std::move(std::make_unique<ObstacleAnimationController>(owner));
}