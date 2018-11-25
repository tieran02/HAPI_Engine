#pragma once
#include "Component.hpp"
#include <tuple>
#include <unordered_map>

struct AnimationComponent : public Component<AnimationComponent> {
	AnimationComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	std::unordered_map<std::string, std::tuple<std::string,bool>> Animations;
	
};

inline std::shared_ptr<BaseComponent> AnimationComponent::Clone() const
{
	return std::make_shared<AnimationComponent>(*this);
}
#pragma once
