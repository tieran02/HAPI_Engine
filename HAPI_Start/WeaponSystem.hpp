#pragma once
#include "System.hpp"
#include "Vector2.hpp"

class WeaponSystem : public System
{
public:
	WeaponSystem();
	~WeaponSystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;
private:
	void ShootBullet(const std::string& bulletEntity, Vector2f position, Vector2f direction, float velocity,
	                 ECSManager& ecs_manager, const Entity& entity);
};

