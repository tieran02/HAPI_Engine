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

void CollisionSystem::Update(ECSManager& ecsManager, Entity & entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	CollidableComponent* collidable_component = (CollidableComponent*)entity.GetComponent(CollidableComponent::ID).get();

	collidable_component->Collided = false;
	collidable_component->CollidedEntity = nullptr;

	//TODO: make the rect fit to the sprite
	Rect rect(0, 32, 0, 32);
	rect.Translate(Vector2i((int)transform_component->GetPostion().x, (int)transform_component->GetPostion().y));
	if(collidable_component->CollisionID < 0 || !ecsManager.IsCollidable(collidable_component->CollisionID))
	{
		//add collidable to the collision list
		if(collidable_component->CollisionID < 0)
			collidable_component->CollisionID = ecsManager.GetCollisionSystem()->ObjectCount();

		//Set object collision layer
		CollisionObject::CollisionLayer layer = (CollisionObject::CollisionLayer)(collidable_component->Layer);
		unsigned int layerMask = collidable_component->CollideWith;
		ecsManager.GetCollisionSystem()->AddCollisionObject(rect, layer, layerMask, collidable_component->CollisionID);

		//add entity to the colliable map aswell
		ecsManager.AddEntityToCollisionMap(collidable_component->CollisionID, entity.ID());
	}else
	{
		ecsManager.GetCollisionSystem()->UpdateCollisionObject(collidable_component->CollisionID, rect);

		//update rect
		int otherID = ecsManager.GetCollisionSystem()->IsColliding(collidable_component->CollisionID);
		if (otherID >= 0)
		{
			if (transform_component != nullptr)
			{
				collidable_component->Collided = true;
				collidable_component->CollidedEntity = ecsManager.GetCollidableEntity(otherID);

				bool isTrigger = collidable_component->isTrigger;
				//check if the collided entity is a trigger
				if (!isTrigger && collidable_component->CollidedEntity != nullptr)
				{
					CollidableComponent* other_collidable_component = (CollidableComponent*)ecsManager.GetCollidableEntity(otherID)->GetComponent(CollidableComponent::ID).get();
					if (other_collidable_component != nullptr)
						isTrigger = other_collidable_component->isTrigger;
				}

				if(!isTrigger)
					transform_component->SetPosition(collidable_component->LastPos);
			}
		}

		collidable_component->LastPos = transform_component->GetPostion();
	}

}
