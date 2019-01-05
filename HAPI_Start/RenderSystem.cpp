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

void RenderSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
}

void RenderSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
}

void RenderSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void RenderSystem::Update(ECSManager& ecsManager, Entity & entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	SpriteComponent* sprite_component = (SpriteComponent*)entity.GetComponent(SpriteComponent::ID).get();

	//check if the entity has a animation component
	AnimationComponent* animation_component = (AnimationComponent*)entity.GetComponent(AnimationComponent::ID).get();
	bool isAnimation = false;
	if (animation_component != nullptr)
		isAnimation = true;

	if(!isAnimation)
		ecsManager.GetRenderer()->InstanceDraw(entity.ID(), sprite_component->SpriteName, transform_component->GetPostion(), transform_component->GetLastPosition());
	else
	{
		ecsManager.GetRenderer()->InstanceDrawAnimation(entity.ID(), animation_component->currentAnimation, transform_component->GetPostion(), transform_component->GetLastPosition(), animation_component->Frame, animation_component->StartFrame, animation_component->EndFrame);
	}

	if (!entity.IsActive())
	{
		ecsManager.GetRenderer()->RemoveInstance(entity.ID());
	}
}
