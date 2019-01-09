#include "AISystem.hpp"
#include "TransformComponent.hpp"
#include "MotionComponent.hpp"
#include "AIControllerComponent.hpp"
#include "WeaponComponent.hpp"
#include "ECSManager.hpp"
#include "World.hpp"

AISystem::AISystem() : System(TransformComponent::ID | MotionComponent::ID | AIControllerComponent::ID)
{
}


AISystem::~AISystem()
{
}

void AISystem::Setup(ECSManager& ecsManager, Entity& entity)
{
}

void AISystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
	AIControllerComponent* ai_component = (AIControllerComponent*)entity.GetComponent(AIControllerComponent::ID).get();
	ai_component->CurrentPathIndex = 0;
	ai_component->CurrentState = AIControllerComponent::AIState::Moving;
}

void AISystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void AISystem::Update(ECSManager & ecsManager, Entity & entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();
	AIControllerComponent* ai_component = (AIControllerComponent*)entity.GetComponent(AIControllerComponent::ID).get();

	switch (ai_component->CurrentState)
	{
	case AIControllerComponent::AIState::Moving:
		MoveTowardsTarget(*transform_component, *motion_component, *ai_component, ecsManager, entity);
		break;
	case AIControllerComponent::AIState::AtDestination:
		AttackBase(*transform_component, *motion_component, *ai_component, ecsManager, entity);
		break;
	case AIControllerComponent::AIState::FollowPlayer:
		MoveTowardsPlayer(*transform_component, *motion_component, *ai_component, ecsManager, entity);
		break;
	default: ;
	}

}

void AISystem::MoveTowardsTarget(TransformComponent& transform_component, MotionComponent& motion_component,
	AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity)
{
	Vector2f AiPos = transform_component.GetPostion();


	Entity* playerEntity = ecsManager.GetEntity(ecsManager.GetWorld()->GetPlayerID());
	Vector2f directionToPlayer = DirectionToPlayer(transform_component, ecsManager);
	const float distanceToPlayer = std::fabs(directionToPlayer.Magnitude());
	if (playerEntity != nullptr && playerEntity->IsActive())
	{
		//check if the AI has a weapon component
		WeaponComponent* weapon_component = (WeaponComponent*)entity.GetComponent(WeaponComponent::ID).get();
		if (weapon_component != nullptr)
		{
			//Calculate direction to player
			if (distanceToPlayer <= ai_controller.DetectRange * 32)
			{
				directionToPlayer.Normalise();

				weapon_component->Direction = directionToPlayer;
				weapon_component->Fire = true;
			}
		}
	}


	//follow path
	const std::vector<Vector2f>* path{ nullptr };
	switch (ai_controller.Enemy)
	{
	case AIControllerComponent::EnemyType::GreenSlime:
		path = &ecsManager.GetWorld()->GetGreenSlimePath();
		break;
	case AIControllerComponent::EnemyType::Ogre:
		if (distanceToPlayer <= ai_controller.DetectRange * 64)
			ai_controller.CurrentState = AIControllerComponent::AIState::FollowPlayer;
		path = &ecsManager.GetWorld()->GetOgrePath();
		break;
	case AIControllerComponent::EnemyType::Boss:
		if (distanceToPlayer <= ai_controller.DetectRange * 32)
			ai_controller.CurrentState = AIControllerComponent::AIState::FollowPlayer;
		path = &ecsManager.GetWorld()->GetOgrePath(); 
		break;
	default:
		break;
	}

	if (path != nullptr)
	{
		const Vector2f& targetPosition = (*path)[ai_controller.CurrentPathIndex];
		Vector2f Direction = targetPosition - AiPos;
		float distance = std::fabs(Direction.Magnitude());
		//check if the AI is close to the target
		if (distance <= 2.0f)
		{
			ai_controller.CurrentPathIndex++;
		}
		//check if it as desination
		if (distance <= 2.0f  && ai_controller.CurrentPathIndex == path->size())
		{
			ai_controller.CurrentState = AIControllerComponent::AIState::AtDestination;
			motion_component.Velocity = 0.0;
		}
		else {
			Direction.Normalise();
			motion_component.Direction = Direction;
			motion_component.Velocity = ai_controller.MoveSpeed;
		}
	}
}

void AISystem::MoveTowardsPlayer(TransformComponent& transform_component, MotionComponent& motion_component,
	AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity)
{
	Entity* player_entity = ecsManager.GetEntity(ecsManager.GetWorld()->GetPlayerID());
	if(player_entity == nullptr || !player_entity->IsActive())
		return;

	//Calculate direction to player
	Vector2f Direction = DirectionToPlayer(transform_component, ecsManager);
	float distance = std::fabs(Direction.Magnitude());

	if(distance > ai_controller.DetectRange * 64)
	{
		ai_controller.CurrentState = AIControllerComponent::AIState::Moving;
		return;
	}

	Direction.Normalise();
	motion_component.Direction = Direction;
	motion_component.Velocity = ai_controller.MoveSpeed * 2.0f;


	//check if the AI has a weapon component
	WeaponComponent* weapon_component = (WeaponComponent*)entity.GetComponent(WeaponComponent::ID).get();
	if (weapon_component != nullptr)
	{
		//Calculate direction to player
		if (distance <= ai_controller.DetectRange * 32)
		{
			weapon_component->Direction = Direction;
			weapon_component->Fire = true;
		}
	}
}

void AISystem::AttackBase(TransformComponent& transform_component, MotionComponent& motion_component,
	AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity)
{
	Entity* base_entity = ecsManager.GetEntity(ecsManager.GetWorld()->GetBaseID());

	if(base_entity == nullptr)
		return;

	//check if the AI has a weapon component
	WeaponComponent* weapon_component = (WeaponComponent*)entity.GetComponent(WeaponComponent::ID).get();
	if (weapon_component != nullptr)
	{
		//Calculate direction to player
		Vector2f BasePos = ((TransformComponent*)base_entity->GetComponent(TransformComponent::ID).get())->GetPostion();
		Vector2f Direction = BasePos - transform_component.GetPostion();
		Direction.Normalise();

		weapon_component->Direction = Direction;
		weapon_component->Fire = true;
	}
}

Vector2f AISystem::DirectionToPlayer(TransformComponent& transform_component, ECSManager& ecsManager)
{
	Entity* player_entity = ecsManager.GetEntity(ecsManager.GetWorld()->GetPlayerID());
	if (player_entity == nullptr)
		return Vector2f();

	//Calculate direction to player
	Vector2f PlayerPos = ((TransformComponent*)player_entity->GetComponent(TransformComponent::ID).get())->GetPostion();
	return PlayerPos - transform_component.GetPostion();
}