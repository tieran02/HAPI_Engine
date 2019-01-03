#pragma once
#include "Component.hpp"
#include "Vector2.hpp"

struct MotionComponent : public Component<MotionComponent> {
	enum class State
	{
		Idle,
		Walking,
	};

	MotionComponent() : Direction(0.0f, 0.0f), Velocity(0.0f){}
	std::shared_ptr<BaseComponent> Clone() const override;
	float MovementSpeed;
	Vector2f Direction;
	float Velocity;
	State CurrentState{State::Idle};

};

inline std::shared_ptr<BaseComponent> MotionComponent::Clone() const
{
	return std::make_shared<MotionComponent>(*this);
}
