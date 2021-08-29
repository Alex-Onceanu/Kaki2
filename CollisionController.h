#pragma once
#include "EntityController.h"
#include "Event.h"


class CollisionController :
    public EntityController,
    public Listener
{
public:
    CollisionController(Entity* o);
    void LoadInitialData(std::map<std::string, std::string>& ini);

    bool CanProcess(Event* e) override;
    void OnEvent(Event* e) override;

    void Draw(const Position& cameraPos) override;
        
protected:
    Position oldPosition{ 0 };

    bool shouldDrawCollisionBox = false;
};

class CollisionControllerCreator : public EntityControllerCreator
{
public:
    std::unique_ptr<EntityController> operator()(Entity* owner) override;
};