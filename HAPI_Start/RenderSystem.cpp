#include "RenderSystem.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ECSManager.hpp"


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

	if(sprite_component->Type == SpriteType::Sprite)
		ecsManager.GetRenderer()->InstanceDraw(entity.ID(), sprite_component->SpriteName, pos);
	else if (sprite_component->Type == SpriteType::Animated)
	{
		ecsManager.GetRenderer()->InstanceDrawAnimation(entity.ID(), sprite_component->SpriteName, pos, sprite_component->Frame, sprite_component->LastTime, sprite_component->Speed, sprite_component->Loop, sprite_component->EndFrame);
	}
}
