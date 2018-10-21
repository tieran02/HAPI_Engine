#include "GameScene.hpp"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	m_playerSprite = m_renderer->LoadSprite("player", "Data\\playerSprite.tga");
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
	const HAPI_TKeyboardData& keyboard_data = HAPI.GetKeyboardData();

	if (keyboard_data.scanCode['D'])
		m_pos.x += 0.5f;
	else if (keyboard_data.scanCode['A'])
		m_pos.x -= 0.5f;
	if (keyboard_data.scanCode['S'])
		m_pos.y += 0.5f;
	else if (keyboard_data.scanCode['W'])
		m_pos.y -= 0.5f;
}

void GameScene::OnRender()
{
	m_renderer->Blit(*m_backgroundSprite, Vector2i(0, 0));

	m_renderer->Draw(*m_playerSprite, m_pos);
}
