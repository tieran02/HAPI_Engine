#pragma once
#include "Component.hpp"

struct CollidableComponent : public Component<CollidableComponent> {
	CollidableComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	int CollisionID{ -1 };
};

inline std::shared_ptr<BaseComponent> CollidableComponent::Clone() const
{
	return std::make_shared<CollidableComponent>(*this);
}
#pragma once
