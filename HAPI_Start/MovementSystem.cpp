#include "MovementSystem.hpp"
#include "TransformComponent.hpp"
#include "MotionComponent.hpp"
#include "Rect.hpp"
#include "ECSManager.hpp"


MovementSystem::MovementSystem(): System(TransformComponent::ID | MotionComponent::ID)
{
}


MovementSystem::~MovementSystem()
{
}

void MovementSystem::Update(ECSManager& ecsManager, const Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();

	if (motion_component->Velocity > 0.0f && motion_component->CurrentState != MotionComponent::State::Attacking)
		motion_component->CurrentState = MotionComponent::State::Walking;
	else if(motion_component->CurrentState != MotionComponent::State::Attacking)
		motion_component->CurrentState = MotionComponent::State::Idle;

	if (motion_component->Velocity > 0.0f) 
	{
		transform_component->Position += Vector2f(motion_component->Direction.x, motion_component->Direction.y) * motion_component->Velocity;
		transform_component->Direction = motion_component->Direction;
	}
}
