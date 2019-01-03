#pragma once
#include "Component.hpp"
#include "Vector2.hpp"
#include <vector>

struct AIControllerComponent : public Component<AIControllerComponent> {
	AIControllerComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	enum class EnemyType
	{
		GreenSlime
	};

	EnemyType Enemy;
	int CurrentPathIndex{ 0 };
	float DetectRange = 10.0f;
};

inline std::shared_ptr<BaseComponent> AIControllerComponent::Clone() const
{
	return std::make_shared<AIControllerComponent>(*this);
}
