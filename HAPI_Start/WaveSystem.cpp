#include "WaveSystem.hpp"
#include "TransformComponent.hpp"
#include "WaveComponent.hpp"
#include "ECSManager.hpp"
#include "World.hpp"
#include "SpawnerComponent.hpp"
#include "UiManager.hpp"
#include "UiTextElement.hpp"
#include <iomanip>

WaveSystem::WaveSystem() : System(TransformComponent::ID | WaveComponent::ID)
{
}


WaveSystem::~WaveSystem()
{
}

void WaveSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
	WaveComponent* wave_component = (WaveComponent*)entity.GetComponent(WaveComponent::ID).get();

	//Spawn entity spawners for the waves in advance
	for (auto& wave : wave_component->GetWaves())
	{
		//Create spawner
		auto spawnerComponents = ecsManager.MakeComponents({ "TransformComponent", "SpawnerComponent" });
		if (!spawnerComponents.empty())
		{
			SpawnerComponent* spawner_component = (SpawnerComponent*)spawnerComponents[1].get();
			spawner_component->EntityToSpawn = wave.EnemyToSpawn;
			spawner_component->SpawnRate = wave.SpawnRate;
			spawner_component->SpawnLimit = wave.AmountToSpawn;
		}
		//Instantiate entity straight from components instead of the entity factory
		auto spawner = ecsManager.InstantiateEntity(spawnerComponents, "WaveSpawner", wave.SpawnPos);
		//Set the spawner to false
		ecsManager.SetEntityActive(spawner->ID(), false);
		m_waveSpawners[wave.Wave].push_back(spawner);
	}

	//set Current Time
	m_startTime = HAPI.GetTime();
}

void WaveSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
}

void WaveSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void WaveSystem::Update(ECSManager& ecsManager, Entity& entity)
{
	if(m_finished)
		return;

	m_waveTimer = HAPI.GetTime() - m_startTime;


	if (WaveFinished(ecsManager))
	{
		m_waveState = WaveState::Finished;
	}

	UiTextElement* text_element = (UiTextElement*)UiManager::Instance().GetUIElement("WaveCountdownText").get();
	float timeLeft = m_gracePeriod - (m_waveTimer / 1000.0f);

	switch (m_waveState)
	{
	case WaveState::Setup:
		//Update wave UI time till next wave
		if (text_element != nullptr)
		{
			std::ostringstream strs;
			strs << std::fixed << std::setprecision(1) << timeLeft;
			text_element->SetText("Wave starting in " + strs.str());
		}
		if(m_currentWave > m_waveSpawners.size())
		{
			m_currentWave = 1;
		}

		if(m_waveTimer >= m_gracePeriod * 1000)
		{
			SpawnWave(m_currentWave, ecsManager);
		}
		break;
	case WaveState::Started:
		//update UI enemy count
		if (text_element != nullptr)
		{
			text_element->SetText("Enemy Count: " + std::to_string(ecsManager.GetWorld()->GetEnemyCount()));
		}
		break;
	case WaveState::Finished:
		m_startTime = HAPI.GetTime();
		m_currentWave++;
		m_waveState = WaveState::Setup;
		break;
	default:
		break;
	}
}

void WaveSystem::SpawnWave(int wave, ECSManager& ecsManager)
{
	if (m_waveSpawners.find(wave) == m_waveSpawners.end()) {
		m_finished = true;
		std::cout << "Level Completed" << std::endl;
		return;
	}

	std::cout << "Wave " << m_currentWave << "seting up" << std::endl;

	int enemyCount = 0;
	for (auto wave : m_waveSpawners.at(wave))
	{
		ecsManager.SetEntityActive(wave->ID(), true);
		//Get spawn count
		SpawnerComponent* spawner_component = (SpawnerComponent*)wave->GetComponent(SpawnerComponent::ID).get();
		enemyCount += spawner_component->SpawnLimit;

	}

	ecsManager.GetWorld()->SetEnemyCount(enemyCount);
	m_waveState = WaveState::Started;

	//Update wave UI
	UiTextElement* text_element = (UiTextElement*)UiManager::Instance().GetUIElement("WaveText").get();
	if (text_element != nullptr) 
	{
		text_element->SetText("Wave: " + std::to_string(m_currentWave));
	}
	//Update wave UI
	text_element = (UiTextElement*)UiManager::Instance().GetUIElement("WaveCountdownText").get();
	if (text_element != nullptr)
	{
		text_element->SetText("");
	}

	std::cout << "Wave " << m_currentWave << "started" << std::endl;
}

bool WaveSystem::WaveFinished(ECSManager& ecsManager)
{
	return m_waveState == WaveState::Started && ecsManager.GetWorld()->GetEnemyCount() <= 0;
}
