#include "DamageSystem.hpp"
#include "CollidableComponent.hpp"
#include "DamageComponent.hpp"
#include "HealthComponent.hpp"
#include "ECSManager.hpp"
#include "TransformComponent.hpp"
#include "AIControllerComponent.hpp"
#include "World.hpp"
#include "ScoreComponent.hpp"
#include "UiManager.hpp"
#include "UiTextElement.hpp"


DamageSystem::DamageSystem() : System(TransformComponent::ID | CollidableComponent::ID | DamageComponent::ID)
{
}


DamageSystem::~DamageSystem()
{
}

void DamageSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
}

void DamageSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
}

void DamageSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void DamageSystem::Update(ECSManager & ecsManager, Entity & entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	CollidableComponent* collidable_component = (CollidableComponent*)entity.GetComponent(CollidableComponent::ID).get();
	DamageComponent* damage_component = (DamageComponent*)entity.GetComponent(DamageComponent::ID).get();


	if (collidable_component->Collided)
	{
		if (damage_component->DestroyOnHit)
		{
			ecsManager.RemoveEntity(entity.ID());
		}

		//spawn entity on hit if it has one
		if(!damage_component->EntityToSpawnOnHit.empty())
		{
			ecsManager.InstantiateEntity(damage_component->EntityToSpawnOnHit, transform_component->GetPostion());
		}
	}

	//check if the collided entity has a health component
	if (collidable_component->CollidedEntity != nullptr)
	{
	HealthComponent* health_component = (HealthComponent*)collidable_component->CollidedEntity->GetComponent(HealthComponent::ID).get();
		if (health_component != nullptr)
		{
			health_component->TakeDamage(damage_component->Damage);
			if (!health_component->IsAlive())
			{
				//Reduce enemy count by one
				if(collidable_component->CollidedEntity->GetComponent(AIControllerComponent::ID) != nullptr)
				{
					ecsManager.GetWorld()->SetEnemyCount(ecsManager.GetWorld()->GetEnemyCount() - 1);
					//check the enemy has a score component
					ScoreComponent* score_component = (ScoreComponent*)collidable_component->CollidedEntity->GetComponent(ScoreComponent::ID).get();
					if (score_component != nullptr) 
					{
						ecsManager.GetWorld()->AddScore(score_component->GetScore());
						((UiTextElement*)UiManager::Instance().GetUIElement("ScoreText").get())->SetText("Score = " + std::to_string(ecsManager.GetWorld()->GetHighScore()));
					}
				}

				//Spawn random pickups when an entity dies
				double r = ((double)rand() / (RAND_MAX));
				if(r > 0.75)
				{
					//Spawn health pickup
					ecsManager.InstantiateEntity("HealthPickup", transform_component->GetPostion());
				}
			}
		}
	}
}
