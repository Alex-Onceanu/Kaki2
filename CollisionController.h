#pragma once
#include "EntityController.h"
#include "Event.h"


class CollisionController :
    public EntityController,
    public Listener
{
public:
    CollisionController(Entity* o, bool movible_);

    bool CanProcess(Event* e) override;
    void OnEvent(Event* e) override;

    //void Update() override;
    void Draw(const Position& cameraPos) override;
        
protected:
    Position oldPosition{ 0 };

    bool shouldDrawCollisionBox = true;
};

