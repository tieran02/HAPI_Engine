#include "ControllerSystem.hpp"
#include "MotionComponent.hpp"
#include "ControllerComponent.hpp"
#include <HAPI_lib.h>
#include "Input.hpp"
#include "ECSManager.hpp"
#include "TransformComponent.hpp"
#include "WeaponComponent.hpp"

using namespace HAPISPACE;

ControllerSystem::ControllerSystem() : System(MotionComponent::ID | ControllerComponent::ID)
{
}


ControllerSystem::~ControllerSystem()
{
}

void ControllerSystem::Update(ECSManager& ecsManager, Entity & entity)
{
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	//Keyboard Input
	const HAPI_TKeyboardData& keyboard_data = HAPI.GetKeyboardData();

	std::cout << transform_component->GetPostion() << std::endl;

	float moveSpeed = motion_component->MovementSpeed;
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

	const auto mouse_data = HAPI.GetMouseData();
	if (mouse_data.leftButtonDown)
	{
		//Get mouse direction
		Vector2f mouse_pos = Vector2f(mouse_data.x, mouse_data.y);
		Vector2f mouse_world_pos = mouse_pos - Vector2f(ecsManager.GetRenderer()->GetOffset().x, ecsManager.GetRenderer()->GetOffset().y);

		Vector2f direction = mouse_world_pos - transform_component->GetPostion();

		direction.Normalise();
		
		//check if the entity has a weapon
		WeaponComponent* weapon_component = (WeaponComponent*)entity.GetComponent(WeaponComponent::ID).get();
		if(weapon_component != nullptr)
		{
			weapon_component->Fire = true;
			weapon_component->Direction = direction;
		}

	}

	if (keyboard_data.scanCode['Q'])
	{
		ecsManager.RemoveEntity(entity.ID());
	}



	//Xbox controller
	//int playerControllerID = Input::GetPlayerControllerID(1);

	//if (playerControllerID != -1) {
	//	Vector2f dir = Input::JoystickDirection(playerControllerID, 1);
	//	motion_component->Direction = dir;
	//}

	motion_component->Direction.Normalise();

	Vector2f pos = { -transform_component->GetPostion().x + ecsManager.GetRenderer()->GetSize().x/2.0f, -transform_component->GetPostion().y + ecsManager.GetRenderer()->GetSize().y / 2.0f };
	ecsManager.GetRenderer()->SetOffset(pos);

}