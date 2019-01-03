#pragma once
#include "Component.hpp"
#include <tuple>
#include <unordered_map>
#include <HAPI_Types.h>

struct AnimationComponent : public Component<AnimationComponent> {
	AnimationComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	std::unordered_map<std::string, std::tuple<std::string,bool>> Animations;

	std::string currentAnimation;
	HAPISPACE::DWORD LastTime{ 0 };
	bool DestroyOnFinish{ false };
	float Speed{ 1.0f };
	int Frame{ 0 };
	int StartFrame{ 0 };
	int EndFrame{ 0 };
};

inline std::shared_ptr<BaseComponent> AnimationComponent::Clone() const
{
	return std::make_shared<AnimationComponent>(*this);
}
#pragma once
