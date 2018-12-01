#include "RenderSystem.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ECSManager.hpp"
#include "AnimationComponent.hpp"


RenderSystem::RenderSystem(): System(TransformComponent::ID | SpriteComponent::ID)
{
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update(ECSManager& ecsManager, const Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	SpriteComponent* sprite_component = (SpriteComponent*)entity.GetComponent(SpriteComponent::ID).get();
	Vector2i pos = Vector2i((int)transform_component->Position.x, (int)transform_component->Position.y);

	//check if the entity has a animation component
	AnimationComponent* animation_component = (AnimationComponent*)entity.GetComponent(AnimationComponent::ID).get();
	bool isAnimation = false;
	if (animation_component != nullptr)
		isAnimation = true;

	if(!isAnimation)
		ecsManager.GetRenderer()->InstanceDraw(entity.ID(), sprite_component->SpriteName, pos, transform_component->Rotation);
	else
	{
		ecsManager.GetRenderer()->InstanceDrawAnimation(entity.ID(), animation_component->currentAnimation, pos, animation_component->Frame, animation_component->StartFrame, animation_component->EndFrame, transform_component->Rotation);
	}
}
