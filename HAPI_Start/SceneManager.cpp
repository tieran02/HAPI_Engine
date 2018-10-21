#include "SceneManager.hpp"

void SceneManager::LoadScene(const std::string& sceneName, Renderer& renderer)
{
	if (m_scenes[sceneName] == nullptr) {
		std::cerr << "SCENE_MANAGER Scene: " << sceneName << " doesn't exist!\n";
		return;
	}

	if (m_currentScene != nullptr)
		unloadCurrentScene();

	m_currentScene = m_scenes[sceneName].get();
	//set renderer
	m_currentScene->m_renderer = &renderer;

	m_currentScene->OnLoad();
	m_currentScene->OnStart();
}

void SceneManager::ReloadScene()
{
	if (m_currentScene == nullptr)
		return;
	std::string currentSceneName = m_currentScene->m_sceneName;
	Renderer* currentRenderer = m_currentScene->m_renderer;

	unloadCurrentScene();
	LoadScene(currentSceneName, *currentRenderer);
}

void SceneManager::unloadCurrentScene()
{
	if(m_currentScene == nullptr)
		return;
	//unload scene
	m_currentScene->OnUnload();
	//unload renderer
	m_currentScene->m_renderer->Cleanup();
	std::cout << "SCENE_MANAGER Unloaded Scene: " + m_currentScene->m_sceneName << std::endl;

	m_currentScene = nullptr;
}

void SceneManager::updateCurrentScene()
{
	if (m_currentScene == nullptr)
		return;

	m_currentScene->OnUpdate();
}

void SceneManager::renderCurrentScene()
{
	m_currentScene->OnRender();
}
