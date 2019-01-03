#pragma once
#include "Component.hpp"
#include "Vector2.hpp"

struct WeaponComponent : public Component<WeaponComponent> {
	std::shared_ptr<BaseComponent> Clone() const override;

	enum class FireState
	{
		Firing,
		Ready
	};

	std::string EntityToFire;
	Vector2f Direction;
	int Firerate;
	int Ammo;
	FireState CurrentFireState;
	bool Fire{ false };

	int currentCooldownTimer{ 0 };
};

inline std::shared_ptr<BaseComponent> WeaponComponent::Clone() const
{
	return std::make_shared<WeaponComponent>(*this);
}
