#include <cassert>
#include <sstream>
#include <vector>

#include "Input.h"
#include "PlayerMovementController.h"
#include "Entity.h"
#include "EntityController.h"
#include "Position.h"
#include "EventEnum.h"

PlayerMovementController::PlayerMovementController(Entity* o)
	:EntityController(o)
{
	ownerPositionPtr = owner->GetPositionPtr();
	ownerPositionPtr->x = 600;
	ownerPositionPtr->y = 300;

	std::vector<EventEnum> events{
		EventEnum::MOVE_UP,
		EventEnum::MOVE_DOWN,
		EventEnum::MOVE_LEFT,
		EventEnum::MOVE_RIGHT,

		EventEnum::STOP_UP,
		EventEnum::STOP_DOWN,
		EventEnum::STOP_LEFT,
		EventEnum::STOP_RIGHT,

		EventEnum::MC_TELEPORT,
	
	};

	std::vector<void (PlayerMovementController::*)()> functions{
		&PlayerMovementController::MoveUp,
		&PlayerMovementController::MoveDown,
		&PlayerMovementController::MoveLeft,
		&PlayerMovementController::MoveRight,

		&PlayerMovementController::StopUp,
		&PlayerMovementController::StopDown,
		&PlayerMovementController::StopLeft,
		&PlayerMovementController::StopRight,

		&PlayerMovementController::Teleport,
	};

	assert(events.size() == functions.size());

	eventToFunction = std::make_unique<std::map<EventEnum, void(PlayerMovementController::*)()>>();

	for (int i = 0; i < events.size(); i++)
	{
		eventToFunction->insert({ events[i],functions[i] });
	}
}

bool PlayerMovementController::CanProcess(Event* e)
{
	return (eventToFunction->find(e->type) != eventToFunction->end());
}

void PlayerMovementController::OnEvent(Event* e)
{
	auto funcPtr = eventToFunction->find(e->type);
	if (funcPtr != eventToFunction->end())
	{
		void (PlayerMovementController:: * f)() = funcPtr->second;
		(this->*f)();
	}
}

void PlayerMovementController::MoveUp()
{
	if (!move_direction.test(MOVING_UP))
		speed.y -= SPEED_REF.y;

	move_direction.set(MOVING_UP);
}

void PlayerMovementController::MoveDown()
{
	if (!move_direction.test(MOVING_DOWN))
		speed.y += SPEED_REF.y;

	move_direction.set(MOVING_DOWN);
}

void PlayerMovementController::MoveRight()
{
	if (!move_direction.test(MOVING_RIGHT))
		speed.x += SPEED_REF.x;

	move_direction.set(MOVING_RIGHT);
}

void PlayerMovementController::MoveLeft()
{
	if (!move_direction.test(MOVING_LEFT))
		speed.x -= SPEED_REF.x;
	move_direction.set(MOVING_LEFT);
}

void PlayerMovementController::StopUp()
{
	if (move_direction[MOVING_UP])
	{
		speed.y += SPEED_REF.y;
		move_direction[MOVING_UP] = 0;
	}
}

void PlayerMovementController::StopDown()
{
	if (move_direction[MOVING_DOWN])
	{
		speed.y -= SPEED_REF.y;
		move_direction[MOVING_DOWN] = 0;
	}
}

void PlayerMovementController::StopRight()
{
	if (move_direction[MOVING_RIGHT])
	{
		speed.x -= SPEED_REF.x;
		move_direction[MOVING_RIGHT] = 0;
	}
}

void PlayerMovementController::StopLeft()
{
	if (move_direction[MOVING_LEFT])
	{
		speed.x += SPEED_REF.x;
		move_direction[MOVING_LEFT] = 0;
	}
}

void PlayerMovementController::Teleport()
//Pour aller plus vite pour les tests
{
	Position pair = Input::GetMousePos();
	int* rect_x = &owner->GetRectPtr()->x;
	int* rect_y = &owner->GetRectPtr()->y;
	Position cameraPos = *ownerPositionPtr - Position({ *rect_x, *rect_y });

	*rect_x = pair.x;
	*rect_y = pair.y;

	*ownerPositionPtr = Position({ *rect_x,*rect_y }) + cameraPos;
}

void PlayerMovementController::Update()
{
	if (move_direction.any())
	{
		ownerPositionPtr->x += int(speed.x);
		ownerPositionPtr->y += int(speed.y);
	}
}

void PlayerMovementController::LoadInitialData(std::map<std::string, std::string>& ini)
{
	std::stringstream sx (ini["speed_x"]),
		sy (ini["speed_y"]);

	sx >> SPEED_REF.x;
	sy >> SPEED_REF.y;
}

void PlayerMovementController::Draw(const Position& cameraPos)
{
	*owner->GetRectPtr() = *ownerPositionPtr - cameraPos;
}

std::unique_ptr<EntityController> PlayerMovementControllerCreator::operator()(Entity* owner)
{
	return std::move(std::make_unique<PlayerMovementController>(owner));
}
