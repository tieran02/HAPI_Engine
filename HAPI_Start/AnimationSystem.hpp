#pragma once
#include "System.hpp"

struct AnimationComponent;
struct TransformComponent;

class AnimationSystem : public System
{
public:
	AnimationSystem();
	~AnimationSystem();
	void Update(ECSManager& ecsManager, const Entity& entity) override;
private:
	void SetAnimation(const std::string& spriteName, AnimationComponent& animation_component, ECSManager& ecs_manager);
};

