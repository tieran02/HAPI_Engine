#include "AnimationSystem.hpp"
#include "MotionComponent.hpp"
#include "AnimationComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"


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
		sprite_component->Loop = true;
		SetAnimation(std::get<0>(animation_component->Animations["idleLeft"]), std::get<0>(animation_component->Animations["idleRight"]), *sprite_component, *transform_component);
		break;
	case MotionComponent::State::Walking:
		sprite_component->Loop = true;
		SetAnimation(std::get<0>(animation_component->Animations["walkLeft"]), std::get<0>(animation_component->Animations["walkRight"]), *sprite_component, *transform_component);
		break;
	case MotionComponent::State::Attacking:
		sprite_component->Loop = false;
		SetAnimation(std::get<0>(animation_component->Animations["attackLeft"]), std::get<0>(animation_component->Animations["attackRight"]), *sprite_component, *transform_component);

		//check if attack is over
		if (sprite_component->Frame == sprite_component->EndFrame)
			motion_component->CurrentState = MotionComponent::State::Idle;
		break;
	default:

		break;
	}
}

void AnimationSystem::SetAnimation(const std::string& spriteName, SpriteComponent& sprite_component)
{
	if (spriteName != sprite_component.SpriteName)
	{
		sprite_component.SpriteName = spriteName;
		sprite_component.Frame = 0;
		sprite_component.LastTime = 0;
	}
}

void AnimationSystem::SetAnimation(const std::string& leftSpriteName, const std::string& rightSpriteName,
	SpriteComponent& sprite_component, TransformComponent& transform_component)
{
	if (leftSpriteName != sprite_component.SpriteName && rightSpriteName != sprite_component.SpriteName)
	{
		//Right
		if (transform_component.Direction.x >= 0)
		{
			sprite_component.SpriteName = rightSpriteName;
			sprite_component.Frame = 0;
			sprite_component.LastTime = 0;
		}
		else
		{
			sprite_component.SpriteName = leftSpriteName;
			sprite_component.Frame = 0;
			sprite_component.LastTime = 0;
		}
	}

	//check wether the new animation is just the flipped version
	if(sprite_component.SpriteName == leftSpriteName || sprite_component.SpriteName == rightSpriteName)
	{
		//Right
		if (transform_component.Direction.x >= 0)
		{
			sprite_component.SpriteName = rightSpriteName;
		}
		else
		{
			sprite_component.SpriteName = leftSpriteName;
		}
	}
}
