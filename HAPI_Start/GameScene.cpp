#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

using namespace HAPISPACE;

void GameScene::OnLoad()
{
	//Load game sprites
	m_renderer->LoadTexture("playerTexture", "Data\\playerSprite.tga");
	//m_renderer->LoadTexture("backgroundTexture","Data\\gameBackground.jpg");
	m_renderer->LoadTexture("rifleMovementTexture", "Data\\RifleMovement.png");
	m_renderer->LoadTexture("testTexture", "Data\\testTiles.png");

	m_renderer->LoadTexture("dungeonTexture", "Data\\tiles_dungeon_v1.1.png");


	m_renderer->LoadSprite("playerSprite", "playerTexture");
	m_renderer->LoadSprite("backgroundSprite", "backgroundTexture");
	m_renderer->LoadAnimatedSprite("rifleIdle", "rifleMovementTexture", 6, 9, 0, 19);
	m_renderer->LoadAnimatedSprite("rifleMove", "rifleMovementTexture", 6, 9, 20, 39);
	m_renderer->LoadAnimatedSprite("rifleAttack", "rifleMovementTexture", 6, 9, 40, 42);

	m_renderer->LoadAnimatedSprite("testAnimation", "testTexture", 8, 2, 0, 16);

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
