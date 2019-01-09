#pragma once
#include "Scene.hpp"
#include "Vector2.hpp"

class MenuScene : public Scene
{
public:
	MenuScene() : Scene("MenuScene") {}

	void OnLoad() override;
	void OnUnload() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
};

