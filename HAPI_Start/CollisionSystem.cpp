#include "CollisionSystem.hpp"
#include "TransformComponent.hpp"
#include "CollidableComponent.hpp"
#include "ECSManager.hpp"


CollisionSystem::CollisionSystem(): System(TransformComponent::ID | CollidableComponent::ID)
{
}


CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Update(ECSManager& ecsManager, const Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	CollidableComponent* collidable_component = (CollidableComponent*)entity.GetComponent(CollidableComponent::ID).get();

	Rect rect(0, 32, 0, 32);
	rect.Translate(Vector2i((int)transform_component->Position.x, (int)transform_component->Position.y));
	if(collidable_component->CollisionID < 0)
	{
		//add collidable to the collision list
		collidable_component->CollisionID = ecsManager.GetCollisionSystem()->ObjectCount();
		ecsManager.GetCollisionSystem()->AddCollisionObject(rect, CollisionLayer::World, collidable_component->CollisionID);

		//add entity to the colliable map aswell
		ecsManager.AddEntityToCollisionMap(collidable_component->CollisionID, entity.ID());
	}else
	{
		//update rect
		ecsManager.GetCollisionSystem()->UpdateCollisionObject(collidable_component->CollisionID, rect);
	}


}
