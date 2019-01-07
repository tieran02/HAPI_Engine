#pragma once
#pragma once
#include "Component.hpp"
#include <string>
#include <vector>
#include "Vector2.hpp"

struct WaveData
{
	int Wave;
	std::string EnemyToSpawn;
	Vector2f SpawnPos;
	int AmountToSpawn;
	float SpawnRate;
};

struct WaveComponent : public Component<WaveComponent> {
	WaveComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;
private:
	std::vector<WaveData> waves;

public:
	void AddWave(const WaveData& wave_data);
	const std::vector<WaveData>& GetWaves() const { return waves; }
};

inline std::shared_ptr<BaseComponent> WaveComponent::Clone() const
{
	return std::make_shared<WaveComponent>(*this);
}

inline void WaveComponent::AddWave(const WaveData& wave_data)
{
	waves.push_back(wave_data);
}
