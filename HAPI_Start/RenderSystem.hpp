#pragma once
#include "System.hpp"
#include <HAPI_Types.h>

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Update(ECSManager& ecsManager, const Entity& entity) override;
	HAPISPACE::DWORD m_lastTime{ 0 };
};

