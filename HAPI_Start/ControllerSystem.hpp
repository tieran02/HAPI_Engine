#pragma once
#include "System.hpp"
#include "Vector2.hpp"

class ControllerSystem : public System
{
public:
	ControllerSystem();
	~ControllerSystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;

};

