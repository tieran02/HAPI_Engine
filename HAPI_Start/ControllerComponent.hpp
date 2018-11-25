#pragma once
#pragma once
#include "Component.hpp"
#include "Vector2.hpp"

struct ControllerComponent : public Component<ControllerComponent> {
	ControllerComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;
	int ControllerID{ -1 };
};

inline std::shared_ptr<BaseComponent> ControllerComponent::Clone() const
{
	return std::make_shared<ControllerComponent>(*this);
}
