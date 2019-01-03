#pragma once
#include "Component.hpp"
#include "Vector2.hpp"

struct CollidableComponent : public Component<CollidableComponent> {

	enum CollisionLayer
	{
		None = 0,
		Player = 1,
		Enemy = 2,
		World = 4,
		Effect = 8,
		Half_Cover = 16
	};

	CollidableComponent();
	std::shared_ptr<BaseComponent> Clone() const override;

	int CollisionID{ -1 };
	CollisionLayer Layer;
	unsigned int CollideWith;
	Vector2f LastPos;
	bool Collided{ false };
	Entity* CollidedEntity{ nullptr };
	bool isTrigger{ false };	
};

inline CollidableComponent::CollidableComponent()
{
	CollideWith = CollisionLayer::Player |
		CollisionLayer::Enemy |
		CollisionLayer::World |
		CollisionLayer::Effect |
		CollisionLayer::Half_Cover; 
}

inline std::shared_ptr<BaseComponent> CollidableComponent::Clone() const
{
	return std::make_shared<CollidableComponent>(*this);
}
#pragma once
