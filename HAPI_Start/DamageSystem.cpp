#include "DamageSystem.hpp"
#include "CollidableComponent.hpp"
#include "DamageComponent.hpp"
#include "HealthComponent.hpp"
#include "ECSManager.hpp"


DamageSystem::DamageSystem() : System(CollidableComponent::ID | DamageComponent::ID)
{
}


DamageSystem::~DamageSystem()
{
}

void DamageSystem::Update(ECSManager & ecsManager, Entity & entity)
{
	CollidableComponent* collidable_component = (CollidableComponent*)entity.GetComponent(CollidableComponent::ID).get();
	DamageComponent* damage_component = (DamageComponent*)entity.GetComponent(DamageComponent::ID).get();


	if (collidable_component->Collided)
	{
		if (damage_component->DestroyOnHit)
		{
			if (ecsManager.HasEntityPool(entity.GetName()))
				entity.SetActive(false);
			else
				ecsManager.RemoveEntity(entity.ID());
		}
	}

	//check if the collided entity has a health component
	if (collidable_component->CollidedEntity != nullptr)
	{
		HealthComponent* health_component = (HealthComponent*)entity.GetComponent(HealthComponent::ID).get();
		if (health_component != nullptr)
		{
			health_component->Health -= damage_component->Damage;
			if (health_component->Health <= 0)
				health_component->Alive = false;
		}
	}
}
