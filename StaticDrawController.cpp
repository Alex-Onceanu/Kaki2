#include "StaticDrawController.h"
#include "Entity.h"

StaticDrawController::StaticDrawController(Entity* o, TextureManager* tm_, const std::string path)
	: EntityController(o)
	, tm(tm_)
{
	image = tm->GetTexture(path);
	
	owner_pos = owner->GetPositionPtr();

	owner->GetSizePtr(&owner_w, &owner_h);
	auto r = Renderer::GetRect(image.get());
	*owner_w = r.w;
	*owner_h = r.h;
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