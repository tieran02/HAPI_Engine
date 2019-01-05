#pragma once
#include "System.hpp"
#include <HAPI_Types.h>

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;
	HAPISPACE::DWORD m_lastTime{ 0 };
};

