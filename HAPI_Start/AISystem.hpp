#pragma once
#include "System.hpp"
#include "TransformComponent.hpp"
#include "MotionComponent.hpp"
#include "AIControllerComponent.hpp"

class AISystem : public System
{
public:
	AISystem();
	~AISystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;

private:
	void MoveTowardsTarget(TransformComponent& transform_component, MotionComponent& motion_component, AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity);
	void MoveTowardsPlayer(TransformComponent& transform_component, MotionComponent& motion_component, AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity);
	void AttackBase(TransformComponent& transform_component, MotionComponent& motion_component, AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity);

	Vector2f DirectionToPlayer(TransformComponent& transform_component, ECSManager& ecsManager);
};

