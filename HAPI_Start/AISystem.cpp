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
		Move(*transform_component, *motion_component, *ai_component, ecsManager, entity);
		break;
	case AIControllerComponent::AIState::AtDestination:
		AttackBase(*transform_component, *motion_component, *ai_component, ecsManager, entity);
		break;
	default: ;
	}

}

void AISystem::Move(TransformComponent& transform_component, MotionComponent& motion_component,
	AIControllerComponent& ai_controller, ECSManager& ecsManager, Entity& entity)
{
	Vector2f AiPos = transform_component.GetPostion();


	int playerID = ecsManager.GetWorld()->GetPlayerID();
	Entity* playerEntity = ecsManager.GetEntity(playerID);
	if (playerEntity != nullptr && playerEntity->IsActive())
	{
		//check if the AI has a weapon component
		WeaponComponent* weapon_component = (WeaponComponent*)entity.GetComponent(WeaponComponent::ID).get();
		if (weapon_component != nullptr)
		{
			//Calculate direction to player
			Vector2f PlayerPos = ((TransformComponent*)playerEntity->GetComponent(TransformComponent::ID).get())->GetPostion();
			Vector2f Direction = PlayerPos - AiPos;
			float distance = std::fabs(Direction.Magnitude());
			if (distance <= ai_controller.DetectRange * 32)
			{
				Direction.Normalise();

				weapon_component->Direction = Direction;
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
		Vector2f PlayerPos = ((TransformComponent*)base_entity->GetComponent(TransformComponent::ID).get())->GetPostion();
		Vector2f Direction = PlayerPos - transform_component.GetPostion();
		Direction.Normalise();

		weapon_component->Direction = Direction;
		weapon_component->Fire = true;
	}
}
