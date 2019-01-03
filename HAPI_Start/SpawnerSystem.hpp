#pragma once
#include "System.hpp"

class SpawnerSystem :public System
{
public:
	SpawnerSystem();
	~SpawnerSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
};

