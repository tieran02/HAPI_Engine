#pragma once
#include "System.hpp"
#include "SpriteComponent.hpp"

struct TransformComponent;

class AnimationSystem : public System
{
public:
	AnimationSystem();
	~AnimationSystem();
	void Update(ECSManager& ecsManager, const Entity& entity) override;
private:
	void SetAnimation(const std::string& spriteName, SpriteComponent& sprite_component);
	void SetAnimation(const std::string& leftSpriteName, const std::string& rightSpriteName, SpriteComponent& sprite_component, TransformComponent& transform_component);
};

