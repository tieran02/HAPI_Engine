#pragma once
#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

struct TransformComponent : public Component<TransformComponent> {
	friend class MovementSystem;
	TransformComponent() : Position(0,0){}
	std::shared_ptr<BaseComponent> Clone() const override;

private:
	Vector2f Position;
	Vector2f LastPosition;
public:
	Vector2f Direction;
	float Rotation{ 0.0f };

	void SetPosition(const Vector2f& pos)
	{
		Position = pos;
	}

	void InitilisePosition(const Vector2f& pos)
	{
		LastPosition = pos;
		Position = pos;
	}

	const Vector2f& GetPostion() const { return Position; }
	const Vector2f& GetLastPosition() const { return LastPosition; }
};

inline std::shared_ptr<BaseComponent> TransformComponent::Clone() const
{
	return std::make_shared<TransformComponent>(*this);
}