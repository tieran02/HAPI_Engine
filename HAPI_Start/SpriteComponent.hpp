#pragma once
#include "Component.hpp"

#include "HAPI_lib.h"

enum class SpriteType
{
	Sprite,
	Animated,
	Tile
};

struct SpriteComponent : public Component<SpriteComponent> {
	std::string SpriteName;
	SpriteType Type;
	float Speed;

	bool Loop{ true };
	int Frame{ 0 };
	int EndFrame{ 0 };
	HAPISPACE::DWORD LastTime{ 0 };

	void SetSprite(const std::string& spriteName);
	std::shared_ptr<BaseComponent> Clone() const override;

};

inline void SpriteComponent::SetSprite(const std::string& spriteName)
{
	if(SpriteName == spriteName)
		return;

	SpriteName = spriteName;
	Frame = 0;
	LastTime = 0;
}

inline std::shared_ptr<BaseComponent> SpriteComponent::Clone() const
{
	return std::make_shared<SpriteComponent>(*this);
}
