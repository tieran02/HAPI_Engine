#pragma once
#include "System.hpp"
#include "Vector2.hpp"

class ControllerSystem : public System
{
public:
	ControllerSystem();
	~ControllerSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
private:
	void ShootBullet(const std::string& bulletEntity, Vector2f position, Vector2f direction, float velocity, ECSManager& ecs_manager);
	bool shot{ false };
};

