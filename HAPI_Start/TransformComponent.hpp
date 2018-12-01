#pragma once
#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

struct TransformComponent : public Component<TransformComponent> {
	TransformComponent() : Position(0,0){}
	std::shared_ptr<BaseComponent> Clone() const override;

	Vector2f Position;
	Vector2f Direction;
	float Rotation{ 0.0f };
};

inline std::shared_ptr<BaseComponent> TransformComponent::Clone() const
{
	return std::make_shared<TransformComponent>(*this);
}