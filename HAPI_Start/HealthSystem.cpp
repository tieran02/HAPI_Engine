#include "HealthSystem.hpp"
#include "TransformComponent.hpp"
#include "HealthComponent.hpp"
#include "ECSManager.hpp"
#include "World.hpp"


HealthSystem::HealthSystem() : System(TransformComponent::ID | HealthComponent::ID)
{
}


HealthSystem::~HealthSystem()
{
}

void HealthSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
	HealthComponent* health_component = (HealthComponent*)entity.GetComponent(HealthComponent::ID).get();
	health_component->SetHealth(health_component->GetMaxHealth());
}

void HealthSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
	HealthComponent* health_component = (HealthComponent*)entity.GetComponent(HealthComponent::ID).get();
	health_component->SetHealth(health_component->GetMaxHealth());
}

void HealthSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
	ecsManager.GetRenderer()->RemoveInstance(entity.ID() + 20000);
}

void HealthSystem::Update(ECSManager& ecsManager, Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	HealthComponent* health_component = (HealthComponent*)entity.GetComponent(HealthComponent::ID).get();

	if(health_component->GetHealth() <= 0)
	{
		if(entity.GetName() == "Player" || entity.GetName() == "Objective")
		{
			ecsManager.GetWorld()->GameOver();
		}
		ecsManager.SetEntityActive(entity.ID(), false);

	}

	Vector2f pos = transform_component->GetPostion() - Vector2f(0, 40);
	Vector2f lastPos = transform_component->GetLastPosition() - Vector2f(0, 40);

	//calulate percentage
	float percentage = health_component->GetHealth() / health_component->GetMaxHealth();

	ecsManager.GetRenderer()->InstanceDraw(entity.ID() + 20000, m_healthBarSprite, pos, lastPos, percentage);
}
