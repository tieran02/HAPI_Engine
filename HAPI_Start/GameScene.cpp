#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "Input.h"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	//Load game sprites
	m_renderer->LoadTexture("player", "Data\\alphaThing.tga");
	m_renderer->LoadTexture("background","Data\\gameBackground.jpg");
	m_renderer->LoadTexture("animated", "Data\\animatedSpritesheet.png");

	m_renderer->LoadSprite("player", m_playerSprite);
	m_renderer->LoadSprite("background", m_backgroundSprite);
	m_renderer->LoadAnimatedSprite("animated", m_runAnimation, 8, 9, 4, 8);
}

void GameScene::OnUnload()
{

}

void GameScene::OnStart()
{
	m_center = Rect(m_renderer->GetSize().x / 2 - 100, m_renderer->GetSize().x / 2 + 100, m_renderer->GetSize().y / 2 - 100, m_renderer->GetSize().y / 2 + 100);

	m_pos = { 0,0 };
}

void GameScene::OnUpdate()
{
	//check for controller input
	if (HAPI.GetTime() % 5000 == 0)
		Input::CheckControllers();


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

	int playerControllerID = Input::GetPlayerControllerID(1);

	if (playerControllerID != -1) {
		Vector2f dir = Input::JoystickDirection(playerControllerID, 1);
		m_pos += dir * movespeed * deltaTime;

		//center rect
		if (m_center.Intersect(m_pos))
		{
			HAPI.SetControllerRumble(Input::GetPlayerControllerID(1), 65535, 65535);
		}
		else
			HAPI.SetControllerRumble(Input::GetPlayerControllerID(1), 0, 0);
	}

	m_lastTime = HAPI.GetTime() / 1000.0f;
}

void GameScene::OnRender()
{
	m_renderer->Draw(m_backgroundSprite, Vector2i(0, 0));


	m_renderer->Draw(m_runAnimation, m_pos, m_currentFrame, .075f);
}
