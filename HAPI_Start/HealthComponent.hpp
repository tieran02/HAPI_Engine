#pragma once
#include "Component.hpp"

struct HealthComponent : public Component<HealthComponent> {
	HealthComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	float Health;
	bool Alive;
};

inline std::shared_ptr<BaseComponent> HealthComponent::Clone() const
{
	return std::make_shared<HealthComponent>(*this);
}
