#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	//Load game sprites
	m_renderer->LoadTexture("playerTexture", "Data\\playerSprite.tga");
	//m_renderer->LoadTexture("backgroundTexture","Data\\gameBackground.jpg");
	m_renderer->LoadTexture("mageIdleTexture", "Data\\MageIdle.png");
	m_renderer->LoadTexture("mageWalkTexture", "Data\\MageWalk.png");
	m_renderer->LoadTexture("mageAttackTexture", "Data\\MageAttack.png");

	m_renderer->LoadTexture("dungeonTexture", "Data\\tiles_dungeon_v1.1.png");


	m_renderer->LoadSprite("playerSprite", "playerTexture");
	m_renderer->LoadSprite("backgroundSprite", "backgroundTexture");
	m_renderer->LoadAnimatedSprite("mageIdleLeft", "mageIdleTexture", 10, 2, 0, 9);
	m_renderer->LoadAnimatedSprite("mageIdleRight", "mageIdleTexture", 10, 2, 10, 20);
	m_renderer->LoadAnimatedSprite("mageWalkLeft", "mageWalkTexture", 8, 2, 0, 7);
	m_renderer->LoadAnimatedSprite("mageWalkRight", "mageWalkTexture", 8, 2, 8, 16);
	m_renderer->LoadAnimatedSprite("mageAttackLeft", "mageAttackTexture", 17, 2, 0, 16);
	m_renderer->LoadAnimatedSprite("mageAttackRight", "mageAttackTexture", 17, 2, 17, 34);
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
	m_renderer->DrawInstancedSprites();
}
