#pragma once
#include "Scene.hpp"
#include "Vector2.hpp"
#include "Sprite.hpp"

class GameScene : public Scene
{
public:
	GameScene() : Scene("GameScene"){}

	void OnLoad() override;
	void OnUnload() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;

private:
	Sprite* m_playerSprite{nullptr};
	Sprite* m_backgroundSprite{nullptr};
	Sprite* m_animatedSprite{ nullptr };

	Vector2f m_pos{ 10,128 };

	float m_lastTime{ 0.0f };
};

