#include "AnimationSystem.hpp"
#include "MotionComponent.hpp"
#include "AnimationComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ECSManager.hpp"


AnimationSystem::AnimationSystem() : System(TransformComponent::ID | MotionComponent::ID | AnimationComponent::ID | SpriteComponent::ID)
{
}


AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update(ECSManager& ecsManager, const Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();
	AnimationComponent* animation_component = (AnimationComponent*)entity.GetComponent(AnimationComponent::ID).get();
	SpriteComponent* sprite_component = (SpriteComponent*)entity.GetComponent(SpriteComponent::ID).get();

	switch (motion_component->CurrentState)
	{
	case MotionComponent::State::Idle:
		SetAnimation(std::get<0>(animation_component->Animations["idle"]), *animation_component, ecsManager);
		break;
	case MotionComponent::State::Walking:
		SetAnimation(std::get<0>(animation_component->Animations["walk"]), *animation_component, ecsManager);
		break;
	case MotionComponent::State::Attacking:
		SetAnimation(std::get<0>(animation_component->Animations["attack"]), *animation_component, ecsManager);

		//check if attack is over
		if (animation_component->Frame == animation_component->EndFrame - animation_component->StartFrame - 1)
			motion_component->CurrentState = MotionComponent::State::Idle;
		break;
	default:

		break;
	}

	//update frame
	if(animation_component->currentAnimation.empty())
		return;

	int milliSpeed = animation_component->Speed * 1000.0f;
	int currentTime = HAPI.GetTime();
	if (currentTime >= animation_component->LastTime + milliSpeed) {
		animation_component->Frame = (animation_component->Frame + 1) % (animation_component->EndFrame - animation_component->StartFrame);
		animation_component->LastTime = HAPI.GetTime();
	}
}

void AnimationSystem::SetAnimation(const std::string& spriteName, AnimationComponent& animation_component, ECSManager& ecs_manager)
{
	if (spriteName != animation_component.currentAnimation)
	{
		animation_component.Frame = 0;
		animation_component.LastTime = 0;
		animation_component.currentAnimation = spriteName;
		ecs_manager.GetRenderer()->GetAnimationFrameData(spriteName, animation_component.StartFrame, animation_component.EndFrame);
	}
}
