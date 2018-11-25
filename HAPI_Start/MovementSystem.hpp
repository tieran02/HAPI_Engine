#pragma once
#include "System.hpp"

class MovementSystem :public System
{
public:
	MovementSystem();
	~MovementSystem();
	void Update(ECSManager& ecsManager, const Entity& entity) override;
};

