#pragma once
#include "System.hpp"

struct AnimationComponent;
struct TransformComponent;

class AnimationSystem : public System
{
public:
	AnimationSystem();
	~AnimationSystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;
private:
	bool m_loop{ true };
	void SetAnimation(const std::string& spriteName, AnimationComponent& animation_component, ECSManager& ecs_manager);
};

