#pragma once
#include "Component.hpp"


struct SpriteComponent : public Component<SpriteComponent> {
private:
	std::string SpriteName;
	float Percentage{ 1.0f };
public:
	void SetSprite(const std::string& spriteName);
	const std::string& GetSprite() const { return SpriteName; }

	void SetPercentage(float percentage) { Percentage = percentage; }
	float GetPercentage() const { return Percentage; }
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
