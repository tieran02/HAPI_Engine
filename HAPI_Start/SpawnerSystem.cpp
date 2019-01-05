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

	spawner_component->CurrentTime++;

	if(spawner_component->CurrentTime >= spawner_component->SpawnRate && spawner_component->CurrentSpawned <= spawner_component->SpawnLimit)
	{
		ecsManager.InstantiateEntity(spawner_component->EntityToSpawn, transform_component->GetPostion());
		spawner_component->CurrentSpawned++;
		spawner_component->CurrentTime = 0;
	}
}
