#pragma once
#include "Component.hpp"
#include <string>

struct SpawnerComponent : public Component<SpawnerComponent> {
	SpawnerComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	std::string EntityToSpawn;
	unsigned int SpawnLimit;
	unsigned int SpawnRate;
	unsigned int CurrentTime{ 0 };
	unsigned int CurrentSpawned{ 0 };
};

inline std::shared_ptr<BaseComponent> SpawnerComponent::Clone() const
{
	return std::make_shared<SpawnerComponent>(*this);
}
