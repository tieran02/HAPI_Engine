#pragma once
#include <string>
#include "Renderer.hpp"

class SceneManager;

class Scene
{
	friend SceneManager;
	friend Game;
public:
	Scene(const std::string& name) : m_sceneName(name){}
	virtual ~Scene(){}

	virtual void OnLoad() = 0;
	virtual void OnUnload() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
protected:
	std::string m_sceneName;
	Renderer* m_renderer;
};

