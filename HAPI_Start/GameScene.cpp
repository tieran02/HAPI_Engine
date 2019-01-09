#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "UiManager.hpp"
#include "Utilities.hpp"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	//Load game sprites
	m_renderer->LoadTexture("PlayerMovementTexture", "Data\\PlayerMovement.png");
	m_renderer->LoadTexture("greenBulletTexture", "Data\\greenBullet.png");
	m_renderer->LoadTexture("purpleBulletTexture", "Data\\purpleBullet.png");
	m_renderer->LoadTexture("bulletTexture", "Data\\bullet.png");
	m_renderer->LoadTexture("explosionTexture", "Data\\explosion.png");
	m_renderer->LoadTexture("greenExplosionTexture", "Data\\greenExplosion.png");
	m_renderer->LoadTexture("purpleExplosionTexture", "Data\\purpleExplosion.png");
	m_renderer->LoadTexture("slimeTexture", "Data\\slime.png");
	m_renderer->LoadTexture("purpleSlimeTexture", "Data\\purpleSlime.png");
	m_renderer->LoadTexture("ogreTexture", "Data\\Ogre.png");
	m_renderer->LoadTexture("healthTexture", "Data\\HealthBar.png");
	m_renderer->LoadTexture("healthPickupTexture", "Data\\HealthPickup.png");
	m_renderer->LoadTexture("dungeonTexture", "Data\\tiles_dungeon_v1.1.png");
	m_renderer->LoadTexture("bossTexture", "Data\\WizzardMove.png");


	m_renderer->LoadSprite("bulletSprite", "bulletTexture");
	m_renderer->LoadSprite("greenBulletSprite", "greenBulletTexture");
	m_renderer->LoadSprite("purpleBulletSprite", "purpleBulletTexture");
	m_renderer->LoadSprite("healthSprite", "healthTexture");


	m_renderer->LoadAnimatedSprite("playerRunRight", "PlayerMovementTexture", 6, 3, 0, 5);
	m_renderer->LoadAnimatedSprite("playerRunLeft", "PlayerMovementTexture", 6, 3, 6, 11);
	m_renderer->LoadAnimatedSprite("playerIdleRight", "PlayerMovementTexture", 6, 3, 12, 14);
	m_renderer->LoadAnimatedSprite("playerIdleLeft", "PlayerMovementTexture", 6, 3, 15, 18);

	m_renderer->LoadAnimatedSprite("ogreRunRight", "ogreTexture", 4, 4, 0, 3);
	m_renderer->LoadAnimatedSprite("ogreRunLeft", "ogreTexture", 4, 4, 4, 7);
	m_renderer->LoadAnimatedSprite("ogreIdleRight", "ogreTexture", 4, 4, 8, 11);
	m_renderer->LoadAnimatedSprite("ogreIdleLeft", "ogreTexture", 4, 4, 12, 15);

	m_renderer->LoadAnimatedSprite("bossIdleLeft", "bossTexture", 3, 2, 0, 2);
	m_renderer->LoadAnimatedSprite("bossIdleRight", "bossTexture", 3, 2, 3, 5);
	m_renderer->LoadAnimatedSprite("bossRunLeft", "bossTexture", 3, 2, 0, 2);
	m_renderer->LoadAnimatedSprite("bossRunRight", "bossTexture", 3, 2, 3, 5);

	m_renderer->LoadAnimatedSprite("healthPickup", "healthPickupTexture", 4, 1, 0, 3);

	m_renderer->LoadAnimatedSprite("explosionAnimation", "explosionTexture", 4, 3, 0, 8);
	m_renderer->LoadAnimatedSprite("greenExplosionAnimation", "greenExplosionTexture", 4, 3, 0, 8);
	m_renderer->LoadAnimatedSprite("purpleExplosionAnimation", "purpleExplosionTexture", 4, 3, 0, 8);
	m_renderer->LoadAnimatedSprite("slimeAnimation", "slimeTexture", 4, 1, 0, 3);
	m_renderer->LoadAnimatedSprite("purpleSlimeAnimation", "purpleSlimeTexture", 4, 1, 0, 3);

	m_renderer->LoadTilesheet("dungeonTilesheet", "dungeonTexture", 20, 24);

	Utilities::LoadSound("gun.wav");
	Utilities::LoadSound("slime.wav");
	Utilities::LoadSound("pickup.wav");

	m_world.LoadLevel(m_renderer);
}

void GameScene::OnUnload()
{
	m_world.UnloadLevel();
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
		SceneManager::Instance().ReloadScene();
	if (keyboard_data.scanCode[HK_ESCAPE])
		SceneManager::Instance().LoadScene("MenuScene",*m_renderer);
}

void GameScene::OnRender()
{
	m_renderer->ClearScreen(50);
	m_world.Render();
	UiManager::Instance().Render();
}
