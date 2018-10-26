#include "GameScene.hpp"
#include "SceneManager.hpp"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	//Load game sprites
	m_playerSprite = m_renderer->LoadSprite("player", "Data\\alphaThing.tga");
	m_backgroundSprite = m_renderer->LoadSprite("background","Data\\gameBackground.jpg");

}

void GameScene::OnUnload()
{

}

void GameScene::OnStart()
{
	m_pos = { 0,0 };
}

void GameScene::OnUpdate()
{

	float deltaTime = (HAPI.GetTime() / 1000.0f) - m_lastTime;

	const HAPI_TKeyboardData& keyboard_data = HAPI.GetKeyboardData();

	if (keyboard_data.scanCode[HK_F5])
		m_sceneManager->ReloadScene();

	float movespeed = 500.0f;
	if (keyboard_data.scanCode['D'])
		m_pos.x += movespeed * deltaTime;
	else if (keyboard_data.scanCode['A'])
		m_pos.x -= movespeed * deltaTime;
	if (keyboard_data.scanCode['S'])
		m_pos.y += movespeed * deltaTime;
	else if (keyboard_data.scanCode['W'])
		m_pos.y -= movespeed * deltaTime;


	m_lastTime = HAPI.GetTime() / 1000.0f;
}

void GameScene::OnRender()
{
	m_renderer->Draw(*m_backgroundSprite, Vector2i(0, 0));

	m_renderer->Draw(*m_playerSprite, m_pos);
}
