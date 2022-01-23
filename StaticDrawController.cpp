#include "StaticDrawController.h"
#include "Entity.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "Utility.h"

StaticDrawController::StaticDrawController(Entity* o)
	: EntityController(o)
{
	owner_pos = owner->GetPositionPtr();

	owner->GetSizePtr(&owner_w, &owner_h);
}

void StaticDrawController::SetPath(const std::string& path)
{
	image = TextureManager::GetTexture(path);
}

void StaticDrawController::LoadInitialData(std::map<std::string, std::string>& ini)
{
	SetPath(ini["path"]);
	*owner_w = StrToInt(ini["width"]);
	*owner_h = StrToInt(ini["height"]);
}

void StaticDrawController::Draw(const Position &cameraPos)
{
	Rect rect = { 
		owner_pos->x - cameraPos.x, 
		owner_pos->y - cameraPos.y, 
		*owner_w, 
		*owner_h 
	};

	Renderer::FullBlit(image.get(), rect);
}

std::unique_ptr<EntityController> StaticDrawControllerCreator::operator()(Entity* owner)
{
	return std::move(std::make_unique<StaticDrawController>(owner));
}