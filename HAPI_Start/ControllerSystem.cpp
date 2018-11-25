#include "ControllerSystem.hpp"
#include "MotionComponent.hpp"
#include "ControllerComponent.hpp"
#include <HAPI_lib.h>
#include "Input.hpp"
#include "ECSManager.hpp"
#include "TransformComponent.hpp"

using namespace HAPISPACE;

ControllerSystem::ControllerSystem() : System(MotionComponent::ID | ControllerComponent::ID)
{
}


ControllerSystem::~ControllerSystem()
{
}

void ControllerSystem::Update(ECSManager& ecsManager, const Entity& entity)
{
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	//Keyboard Input
	const HAPI_TKeyboardData& keyboard_data = HAPI.GetKeyboardData();

	float moveSpeed = 5.0f;
	motion_component->Velocity = 0.0f;
	motion_component->Direction = Vector2f(0, 0);

	//Keyboard
	if (keyboard_data.scanCode['D']) 
	{
		motion_component->Velocity = moveSpeed;
		motion_component->Direction += Vector2f(1, 0);
	}
	if (keyboard_data.scanCode['A']) 
	{
		motion_component->Velocity = moveSpeed;
		motion_component->Direction += Vector2f(-1, 0);
	}
	if (keyboard_data.scanCode['S']) 
	{
		motion_component->Velocity = moveSpeed;
		motion_component->Direction += Vector2f(0, 1);
	}
	if (keyboard_data.scanCode['W'])
	{
		motion_component->Velocity = moveSpeed;
		motion_component->Direction += Vector2f(0, -1);
	}

	if (keyboard_data.scanCode['E'])
	{
		motion_component->CurrentState = MotionComponent::State::Attacking;
	}

	if (keyboard_data.scanCode['Q'])
	{
		ecsManager.RemoveEntity(entity.ID());
	}



	//Xbox controller
	int playerControllerID = Input::GetPlayerControllerID(1);

	if (playerControllerID != -1) {
		Vector2f dir = Input::JoystickDirection(playerControllerID, 1);
		motion_component->Direction = dir;
	}

	motion_component->Direction.Normalise();

	Vector2i pos = { (int)-transform_component->Position.x + ecsManager.GetRenderer()->GetSize().x/2, (int)-transform_component->Position.y + ecsManager.GetRenderer()->GetSize().y / 2 };
	ecsManager.GetRenderer()->SetOffset(pos);

}
