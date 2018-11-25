#pragma once
#include "ECSManager.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Renderer.hpp"
#include "Tilemap.hpp"

class World
{
public:
	World();
	~World();
	void Load(Renderer* renderer);
	void Unload();

	void Update();
	void Render();
	void SpawnEntity(const std::string & entityName, Vector3f pos, Vector2f dir = { 0.0f,0.0f }, float velocity = 0.0f);
private:
	void initilise();
	ECSManager m_ecsManager;
	Renderer* m_renderer;

	Tilemap m_tilemap;

	HAPISPACE::DWORD m_lastTime{0};
};

