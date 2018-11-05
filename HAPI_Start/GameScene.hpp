#pragma once
#include "Scene.hpp"
#include "Vector2.hpp"
#include "Sprite.hpp"
#include "AnimatedSprite.hpp"

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
	Sprite m_playerSprite;
	Sprite m_backgroundSprite;
	AnimatedSprite m_runAnimation;

	Rect m_center;
	Vector2f m_pos{ 10,128 };

	float m_lastTime{ 0.0f };

	int m_currentFrame = 0;

};

