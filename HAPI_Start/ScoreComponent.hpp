#pragma once
#include "Component.hpp"
#include <string>

struct ScoreComponent : public Component<ScoreComponent> {
	ScoreComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

private:
	int m_score{10};
public:
	void SetScore(int score) { m_score = score; }
	int GetScore() const { return m_score; }
};

inline std::shared_ptr<BaseComponent> ScoreComponent::Clone() const
{
	return std::make_shared<ScoreComponent>(*this);
}