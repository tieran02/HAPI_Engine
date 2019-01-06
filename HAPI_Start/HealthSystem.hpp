#pragma once
#include "System.hpp"

class HealthSystem : public System
{
public:
	HealthSystem();
	~HealthSystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;
private:
	std::string m_healthBarSprite{ "healthSprite" };
};

