#pragma once
#include <unordered_map>
#include "Scene.hpp"

class SceneManager
{
	friend Game;
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

private:
	SceneManager() {}
public:

	template<typename T>
	void AddScene();
	void LoadScene(const std::string& sceneName,  Renderer& renderer);
	void ReloadScene();

private:
	std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
	Scene* m_currentScene{ nullptr };

	void unloadCurrentScene();
	void updateCurrentScene();
	void renderCurrentScene();
};

template <typename T>
void SceneManager::AddScene()
{

	auto scene = std::make_unique<T>();
	//check if scene exists in scene map
	if(m_scenes.find(scene->m_sceneName) != m_scenes.end())
		return; // scene already exists

	//add scene to scene map
	m_scenes[scene->m_sceneName] = std::move(scene);
}

