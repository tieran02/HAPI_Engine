#include "PickupSystem.hpp"
#include "TransformComponent.hpp"
#include "PickupComponent.hpp"
#include "CollidableComponent.hpp"
#include "HealthComponent.hpp"
#include "ECSManager.hpp"


PickupSystem::PickupSystem() : System(TransformComponent::ID | PickupComponent::ID | CollidableComponent::ID)
{
}


PickupSystem::~PickupSystem()
{
}

void PickupSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
}

void PickupSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
}

void PickupSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void PickupSystem::Update(ECSManager& ecsManager, Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	PickupComponent* pickup_component = (PickupComponent*)entity.GetComponent(PickupComponent::ID).get();
	CollidableComponent* collidable_component = (CollidableComponent*)entity.GetComponent(CollidableComponent::ID).get();

	if(collidable_component->Collided && collidable_component->CollidedEntity != nullptr)
	{
		if(collidable_component->CollidedEntity->GetName() == "Player")
		{
			HealthComponent* health_component = (HealthComponent*)collidable_component->CollidedEntity->GetComponent(HealthComponent::ID).get();

			switch (pickup_component->GetType())
			{
			case PickupComponent::PickupType::Health:
				if(health_component != nullptr)
				{
					health_component->Heal(pickup_component->GetAmount());
					ecsManager.SetEntityActive(entity.ID(), false);
				}
				break;
			default: 
				return;
			}
		}
	}
}
