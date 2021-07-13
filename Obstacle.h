#pragma once
#include "WorldEntity.h"

class Obstacle :
    public WorldEntity
{
public:
    Obstacle(TextureManager* tm_, const std::string path, Position initialPos);
    ~Obstacle();

    void ProcessCollision();

    void ProcessInput() override;
    void Update() override;
    void Draw(Position cameraPos) override;

protected:
    std::shared_ptr<Texture> image;
};

