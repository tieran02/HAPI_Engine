#pragma once
#include "Component.hpp"
#include "string"

struct DamageComponent : public Component<DamageComponent> {
	DamageComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	int Damage;
	bool DestroyOnHit{ false };
	std::string EntityToSpawnOnHit;
};

inline std::shared_ptr<BaseComponent> DamageComponent::Clone() const
{
	return std::make_shared<DamageComponent>(*this);
}
#pragma once
