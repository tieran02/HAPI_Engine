#pragma once
#include "Component.hpp"
#include "HAPI_lib.h"


struct SpriteComponent : public Component<SpriteComponent> {
	std::string SpriteName;

	void SetSprite(const std::string& spriteName);
	std::shared_ptr<BaseComponent> Clone() const override;

};

inline void SpriteComponent::SetSprite(const std::string& spriteName)
{
	if(SpriteName == spriteName)
		return;

	SpriteName = spriteName;
}

inline std::shared_ptr<BaseComponent> SpriteComponent::Clone() const
{
	return std::make_shared<SpriteComponent>(*this);
}
