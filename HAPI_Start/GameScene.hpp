#pragma once
#include "Scene.hpp"
#include "Vector2.hpp"

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
	
};

