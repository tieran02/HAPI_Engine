#pragma once
#include "Component.hpp"

struct HealthComponent : public Component<HealthComponent> {
	HealthComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;
private:
	float MaxHealth;
	float Health;
	bool Alive;

public:
	void SetMaxHealth(float health)
	{
		MaxHealth = health;
	}

	void SetHealth(float health)
	{
		Health = health;
		if (Health < 0)
			Alive = false;
		else
			Alive = true;
	}

	void TakeDamage(float amount)
	{
		Health -= amount;
		if (Health < 0)
			Alive = false;
		else
			Alive = true;
	}

	void Heal(float amount)
	{
		Health += amount;
		if (Health > MaxHealth)
			Health = MaxHealth;

		if (Health < 0)
			Alive = false;
		else
			Alive = true;
	}

	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }
	bool IsAlive() const { return Alive; }
};

inline std::shared_ptr<BaseComponent> HealthComponent::Clone() const
{
	return std::make_shared<HealthComponent>(*this);
}
