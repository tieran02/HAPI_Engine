#include "WeaponSystem.hpp"
#include "TransformComponent.hpp"
#include "WeaponComponent.hpp"
#include "CollidableComponent.hpp"
#include "ECSManager.hpp"
#include "ControllerComponent.hpp"

WeaponSystem::WeaponSystem() : System(TransformComponent::ID | WeaponComponent::ID)
{

}

WeaponSystem::~WeaponSystem()
{
}

void WeaponSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
}

void WeaponSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
}

void WeaponSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void WeaponSystem::Update(ECSManager& ecsManager, Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	WeaponComponent* weapon_component = (WeaponComponent*)entity.GetComponent(WeaponComponent::ID).get();

	weapon_component->currentCooldownTimer++;

	//check if the weapon should fire
	if(weapon_component->Fire)
	{
		//check current state of the gun
		switch (weapon_component->CurrentFireState)
		{
		case WeaponComponent::FireState::Firing:
			weapon_component->Fire = false;
			//set cooldown period
			if (weapon_component->currentCooldownTimer >= weapon_component->Firerate)
			{
				weapon_component->CurrentFireState = WeaponComponent::FireState::Ready;
				weapon_component->currentCooldownTimer = 0;
			}
			return;
		case WeaponComponent::FireState::Ready:
			//TODO FIRE GUN
			ShootBullet(weapon_component->EntityToFire, transform_component->GetPostion(), weapon_component->Direction, 20, ecsManager, entity);
			weapon_component->CurrentFireState = WeaponComponent::FireState::Firing;
			break;
		default: ;
		}
	}
	weapon_component->Fire = false;
}

void WeaponSystem::ShootBullet(const std::string& bulletEntity, Vector2f position, Vector2f direction, float velocity, ECSManager& ecs_manager, const Entity& entity)
{
	const auto bullet = ecs_manager.InstantiateEntity(bulletEntity, position, direction, velocity);
	if (bullet == nullptr)
		return;
	CollidableComponent* collidable_component = (CollidableComponent*)bullet->GetComponent(CollidableComponent::ID).get();

	//check what side to set the bullet
	if (entity.GetName() == "Player")
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::Enemy | CollidableComponent::CollisionLayer::World);
	else
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::Player | CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Objective);
}
