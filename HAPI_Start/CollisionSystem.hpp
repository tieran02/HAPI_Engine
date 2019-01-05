#pragma once
#include "System.hpp"

class CollisionSystem :public System
{
public:
	CollisionSystem();
	~CollisionSystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;
};

