#include "Game.hpp"
#include <HAPI_lib.h>

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
		update();
	}
}

void Game::LoadScene(const std::string& sceneName)
{
	m_sceneMangager.LoadScene(sceneName,m_renderer);
}

void Game::update()
{
	m_sceneMangager.updateCurrentScene();
	m_sceneMangager.renderCurrentScene();

	//m_sceneMangager.ReloadScene();
}
