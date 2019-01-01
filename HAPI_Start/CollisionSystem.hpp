#pragma once
#include "System.hpp"

class CollisionSystem :public System
{
public:
	CollisionSystem();
	~CollisionSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
};

