#include "AnimationSystem.hpp"
#include "MotionComponent.hpp"
#include "AnimationComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ECSManager.hpp"


AnimationSystem::AnimationSystem() : System(TransformComponent::ID | AnimationComponent::ID | SpriteComponent::ID)
{
}


AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update(ECSManager& ecsManager, Entity & entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();
	AnimationComponent* animation_component = (AnimationComponent*)entity.GetComponent(AnimationComponent::ID).get();
	SpriteComponent* sprite_component = (SpriteComponent*)entity.GetComponent(SpriteComponent::ID).get();

	if(animation_component->Animations.size() == 1)
	{
		auto animationName = animation_component->Animations.begin()->second;
		SetAnimation(std::get<0>(animationName), *animation_component, ecsManager);
	}

	else if(motion_component != nullptr ){
		switch (motion_component->CurrentState)
		{
		case MotionComponent::State::Idle:
			if (transform_component->Direction.x >= 0)
				SetAnimation(std::get<0>(animation_component->Animations["idleRight"]), *animation_component, ecsManager);
			else
				SetAnimation(std::get<0>(animation_component->Animations["idleLeft"]), *animation_component, ecsManager);
			break;
		case MotionComponent::State::Walking:
			if (transform_component->Direction.x >= 0)
				SetAnimation(std::get<0>(animation_component->Animations["walkRight"]), *animation_component, ecsManager);
			else
				SetAnimation(std::get<0>(animation_component->Animations["walkLeft"]), *animation_component, ecsManager);
			break;
		default:

			break;
		}
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


	if(animation_component->DestroyOnFinish)
	{
		if(animation_component->Frame >= animation_component->EndFrame-1)
		{
			ecsManager.RemoveEntity(entity.ID());
		}
	}
}

void AnimationSystem::SetAnimation(const std::string& spriteName, AnimationComponent& animation_component, ECSManager& ecs_manager)
{

	if(spriteName.empty())
	{
		return;
	}

	if (spriteName != animation_component.currentAnimation)
	{
		animation_component.Frame = 0;
		animation_component.LastTime = 0;
		animation_component.currentAnimation = spriteName;
		ecs_manager.GetRenderer()->GetAnimationFrameData(spriteName, animation_component.StartFrame, animation_component.EndFrame);
	}
}
