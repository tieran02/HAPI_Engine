#pragma once
#include "System.hpp"

class DamageSystem : public System
{
public:
	DamageSystem();
	~DamageSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
};

