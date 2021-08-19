#pragma once

#include <bitset>
#include <memory>
#include <map>

#include "EntityController.h"
#include "Event.h"
#include "Position.h"

typedef Pos<double> Speed_t;

constexpr auto MOVING_LEFT = 1;
constexpr auto MOVING_RIGHT = 2;
constexpr auto MOVING_UP = 3;
constexpr auto MOVING_DOWN = 4;

class PlayerMovementController :
    public EntityController,
    public Listener
{
public:
    PlayerMovementController(Entity* o);

    void Update() override;
    void Draw(const Position& cameraPos) override;

    bool CanProcess(Event* e) override;
    void OnEvent(Event* e) override;

private:
    void MoveUp();
    void MoveDown();
    void MoveRight();
    void MoveLeft();

    void StopUp();
    void StopDown();
    void StopRight();
    void StopLeft();

private:

    Position* ownerPositionPtr;

    std::unique_ptr<std::map<EventEnum, void (PlayerMovementController::*)()>> eventToFunction;


    const static int SPEED_REF = 5;
    Speed_t speed = { 0,0 };

    std::bitset<5> move_direction = 0;
};

