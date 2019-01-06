#pragma once
#include "Component.hpp"
#include <string>

struct PickupComponent : public Component<PickupComponent> {
	PickupComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	enum class PickupType
	{
		Health
	};

private:
	float m_amount{ 0 };
	PickupType m_pickupType;
public:
	void SetAmount(float amount) { m_amount = amount; }
	float GetAmount() const { return m_amount; }

	void SetType(PickupType type) { m_pickupType = type; }
	PickupType GetType() const { return m_pickupType; }
};

inline std::shared_ptr<BaseComponent> PickupComponent::Clone() const
{
	return std::make_shared<PickupComponent>(*this);
}