#pragma once
#include "System.hpp"
#include <vector>
#include <unordered_map>
#include <HAPI_Types.h>

enum class WaveState
{
	Setup,
	Started,
	Finished
};

class WaveSystem : public System
{
public:
	WaveSystem();
	~WaveSystem();
	void Setup(ECSManager& ecsManager, Entity& entity) override;
	void OnEnable(ECSManager& ecsManager, Entity& entity) override;
	void OnDisable(ECSManager& ecsManager, Entity& entity) override;
	void Update(ECSManager& ecsManager, Entity & entity) override;
private:
	int m_currentWave{5};
	bool m_setup{ false };
	WaveState m_waveState{WaveState::Setup};
	std::unordered_map<int, std::vector<Entity*>> m_waveSpawners;
	HAPISPACE::DWORD m_startTime{ 0 };
	//Wave Timer in seconds
	HAPISPACE::DWORD m_waveTimer{ 0 };
	//Time between waves in seconds
	float m_gracePeriod{ 10.0f };
	bool m_finished{ false };

	void SpawnWave(int wave, ECSManager& ecsManager);
	bool WaveFinished(ECSManager& ecsManager);
};

