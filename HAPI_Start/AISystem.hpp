#pragma once
#include "System.hpp"

class AISystem : public System
{
public:
	AISystem();
	~AISystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
};

