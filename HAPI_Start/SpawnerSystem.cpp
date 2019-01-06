#include "SpawnerSystem.hpp"
#include "TransformComponent.hpp"
#include "SpawnerComponent.hpp"
#include "ECSManager.hpp"

SpawnerSystem::SpawnerSystem() : System(TransformComponent::ID | SpawnerComponent::ID)
{
}


SpawnerSystem::~SpawnerSystem()
{
}

void SpawnerSystem::Setup(ECSManager& ecsManager, Entity& entity)
{

}

void SpawnerSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
}

void SpawnerSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void SpawnerSystem::Update(ECSManager& ecsManager, Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	SpawnerComponent* spawner_component = (SpawnerComponent*)entity.GetComponent(SpawnerComponent::ID).get();

	if (!spawner_component->SpawnOneAtATime) {
		spawner_component->CurrentTime++;

		if (spawner_component->CurrentTime >= spawner_component->SpawnRate && (spawner_component->CurrentSpawned <= spawner_component->SpawnLimit || spawner_component->SpawnLimit == 0))
		{
			ecsManager.InstantiateEntity(spawner_component->EntityToSpawn, transform_component->GetPostion());
			spawner_component->CurrentSpawned++;
			spawner_component->CurrentTime = 0;
		}
	}
	else
	{
		Entity* lastEntity = nullptr;
		if (spawner_component->LastEntitySpawned >= 0)
			lastEntity = ecsManager.GetEntity(spawner_component->LastEntitySpawned);

		if (lastEntity != nullptr && !lastEntity->IsActive()) {
			spawner_component->LastEntitySpawned = -1;
			lastEntity = nullptr;
		}

		if (lastEntity == nullptr) {
			spawner_component->CurrentTime++;

			if (spawner_component->CurrentTime >= spawner_component->SpawnRate && spawner_component->CurrentSpawned <= spawner_component->SpawnLimit)
			{
				Entity* spawnedEntity = ecsManager.InstantiateEntity(spawner_component->EntityToSpawn, transform_component->GetPostion());
				spawner_component->LastEntitySpawned = spawnedEntity->ID();
				spawner_component->CurrentSpawned++;
				spawner_component->CurrentTime = 0;
			}
		}
	}
}
