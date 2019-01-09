#pragma once
#include <string>
#include "Vector2.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"

class Game
{
public:
	Game(const std::string& name) : m_name(name){}
	~Game();

	void Initialise(const Vector2i& screenSize);
	void Start();

	template <typename T>
	void AddScene()
	{
		SceneManager::Instance().AddScene<T>();
	}

	void LoadScene(const std::string& sceneName);

private:
	std::string m_name;
	Renderer m_renderer;

	void update();
};

