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
}

void AISystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
}

void AISystem::Update(ECSManager & ecsManager, Entity & entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	MotionComponent* motion_component = (MotionComponent*)entity.GetComponent(MotionComponent::ID).get();
	AIControllerComponent* ai_component = (AIControllerComponent*)entity.GetComponent(AIControllerComponent::ID).get();

	Vector2f AiPos = transform_component->GetPostion();


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
			if (distance <= ai_component->DetectRange * 32)
			{
				Direction.Normalise();

				weapon_component->Direction = Direction;
				weapon_component->Fire = true;
			}
		}
	}

	//follow path
	const std::vector<Vector2f>* path{ nullptr };
	switch (ai_component->Enemy)
	{
	case AIControllerComponent::EnemyType::GreenSlime:
		path = &ecsManager.GetWorld()->GetGreenSlimePath();
		break;
	default:
		break;
	}

	if (path != nullptr)
	{
		const Vector2f& targetPosition = (*path)[ai_component->CurrentPathIndex];
		Vector2f Direction = targetPosition - AiPos;
		float distance = std::fabs(Direction.Magnitude());
		//check if the AI is close to the target
		if (distance <= 2.0f && ai_component->CurrentPathIndex <= (*path).size())
		{
			ai_component->CurrentPathIndex++;
		}

		Direction.Normalise();
		motion_component->Direction = Direction;
		motion_component->Velocity = ai_component->MoveSpeed;
	}
}
