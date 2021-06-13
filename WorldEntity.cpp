#include "WorldEntity.h"
#include "Renderer.h"
#include "Position.h"
#include "TextureManager.h"

WorldEntity::WorldEntity(TextureManager* tm_)
	:tm(tm_),
	pos({ 0,0 }),
	rect({ 0,0,0,0 })
{

}

WorldEntity::~WorldEntity()
{

}

void WorldEntity::SetPosition(Position dst)
{
	pos = dst;
}

Position WorldEntity::GetPosition()
{
	return pos;
}

void WorldEntity::AddPosition(int x, int y)
{
	pos.x += x;
	pos.y += y;
}

void WorldEntity::AddPosition(Position src)
{
	pos = pos + src;
}