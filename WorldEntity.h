#pragma once

#include <memory>
#include <string>

#include "Entity.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Position.h"

struct Rect;

class WorldEntity :
    public Entity
{
public:
    WorldEntity(TextureManager* tm_);
    ~WorldEntity();

    virtual void ProcessCollision() = 0;

    virtual void ProcessInput() override = 0;
    virtual void Update() override = 0;
    virtual void Draw() override = 0;

    void SetPosition(Position dst);
    Position GetPosition();

    void AddPosition(int x, int y);
    void AddPosition(Position src);

protected:
    Rect rect;
    Position pos;

    TextureManager* tm;

    bool shouldMirrorBlit;
};

