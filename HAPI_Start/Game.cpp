#include "Game.hpp"
#include <HAPI_lib.h>
#include "Input.hpp"
#include "Time.hpp"

using namespace HAPISPACE;

Game::~Game()
{
}

void Game::Initialise(const Vector2i& screenSize)
{
	m_renderer.Intialise(screenSize,m_name);

	HAPI.SetShowFPS(true);
}

void Game::Start()
{
	while(HAPI.Update())
	{
		Time::Instance().startUpdateTime();
		update();
		Time::Instance().endUpdateTime();
	}
}

void Game::LoadScene(const std::string& sceneName)
{
	SceneManager::Instance().LoadScene(sceneName,m_renderer);
}

void Game::update()
{
	SceneManager::Instance().updateCurrentScene();
	SceneManager::Instance().renderCurrentScene();

}
