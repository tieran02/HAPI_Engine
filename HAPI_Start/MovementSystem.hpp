#pragma once
#include "System.hpp"

class MovementSystem :public System
{
public:
	MovementSystem();
	~MovementSystem();
	void Update(ECSManager& ecsManager, Entity & entity) override;
};

