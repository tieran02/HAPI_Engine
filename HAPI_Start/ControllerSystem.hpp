#pragma once
#include "System.hpp"
#include "Vector2.hpp"

class ControllerSystem : public System
{
public:
	ControllerSystem();
	~ControllerSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;

};

