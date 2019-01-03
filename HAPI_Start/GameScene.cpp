#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	//Load game sprites
	m_renderer->LoadTexture("PlayerMovementTexture", "Data\\PlayerMovement.png");
	m_renderer->LoadTexture("testTexture", "Data\\testTiles.png");
	m_renderer->LoadTexture("greenBulletTexture", "Data\\greenBullet.png");
	m_renderer->LoadTexture("bulletTexture", "Data\\bullet.png");
	m_renderer->LoadTexture("explosionTexture", "Data\\explosion.png");
	m_renderer->LoadTexture("greenExplosionTexture", "Data\\greenExplosion.png");
	m_renderer->LoadTexture("slimeTexture", "Data\\slime.png");

	m_renderer->LoadTexture("dungeonTexture", "Data\\tiles_dungeon_v1.1.png");


	m_renderer->LoadSprite("playerSprite", "playerTexture");
	m_renderer->LoadSprite("backgroundSprite", "backgroundTexture");
	m_renderer->LoadSprite("bulletSprite", "bulletTexture");
	m_renderer->LoadSprite("greenBulletSprite", "greenBulletTexture");


	m_renderer->LoadAnimatedSprite("playerRunRight", "PlayerMovementTexture", 6, 3, 0, 5);
	m_renderer->LoadAnimatedSprite("playerRunLeft", "PlayerMovementTexture", 6, 3, 6, 11);
	m_renderer->LoadAnimatedSprite("playerIdleRight", "PlayerMovementTexture", 6, 3, 12, 14);
	m_renderer->LoadAnimatedSprite("playerIdleLeft", "PlayerMovementTexture", 6, 3, 15, 18);

	m_renderer->LoadAnimatedSprite("testAnimation", "testTexture", 8, 2, 0, 16);

	m_renderer->LoadAnimatedSprite("explosionAnimation", "explosionTexture", 4, 3, 0, 8);
	m_renderer->LoadAnimatedSprite("greenExplosionAnimation", "greenExplosionTexture", 4, 3, 0, 8);
	m_renderer->LoadAnimatedSprite("slimeAnimation", "slimeTexture", 4, 1, 0, 3);

	m_renderer->LoadTilesheet("dungeonTilesheet", "dungeonTexture", 20, 24);

	m_world.Load(m_renderer);
}

void GameScene::OnUnload()
{
	m_world.Unload();
}

void GameScene::OnStart()
{

}

void GameScene::OnUpdate()
{
	//Update world
	m_world.Update();

	const HAPI_TKeyboardData& keyboard_data = HAPI.GetKeyboardData();

	if (keyboard_data.scanCode[HK_F5])
		m_sceneManager->ReloadScene();


}

void GameScene::OnRender()
{
	m_renderer->ClearScreen(50);
	m_world.Render();
}
