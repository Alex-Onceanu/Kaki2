#include "Obstacle.h"

#include <string>

Obstacle::Obstacle(TextureManager* tm_, const std::string path, Position initialPos)
	: WorldEntity(tm_)
{
	image = tm->GetTexture(path);
	rect = Renderer::GetRect(image.get());

	pos = initialPos;
}

Obstacle::~Obstacle()
{

}

void Obstacle::ProcessCollision()
{
	return;
}

void Obstacle::ProcessInput()
{
	return;
}

void Obstacle::Update()
{

}

void Obstacle::Draw(Position cameraPos)
{
	rect.x = pos.x - cameraPos.x;
	rect.y = pos.y - cameraPos.y;

	Renderer::FullBlit(image.get(), rect);
}