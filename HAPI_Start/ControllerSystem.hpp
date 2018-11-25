#pragma once
#include "System.hpp"

class ControllerSystem : public System
{
public:
	ControllerSystem();
	~ControllerSystem();
	void Update(ECSManager& ecsManager, const Entity& entity) override;
};

