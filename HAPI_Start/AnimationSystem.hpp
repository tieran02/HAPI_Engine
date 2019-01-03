#pragma once
#include "System.hpp"

struct AnimationComponent;
struct TransformComponent;

class AnimationSystem : public System
{
public:
	AnimationSystem();
	~AnimationSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
private:
	bool m_loop{ true };
	void SetAnimation(const std::string& spriteName, AnimationComponent& animation_component, ECSManager& ecs_manager);
};

