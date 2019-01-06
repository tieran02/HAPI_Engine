#pragma once
#include "Component.hpp"
#include <string>

struct TileComponent : public Component<TileComponent> {
	TileComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

	std::string SpriteSheet;
	int Index{ 0 };
};

inline std::shared_ptr<BaseComponent> TileComponent::Clone() const
{
	return std::make_shared<TileComponent>(*this);
}