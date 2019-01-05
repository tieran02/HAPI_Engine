#pragma once
#include "ECSManager.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Renderer.hpp"
#include "Tilemap.hpp"
#include "CollisionManager.hpp"

class World
{
public:
	World();
	~World();
	void Load(Renderer* renderer);
	void Unload();

	void Update();
	void Render();
	int SpawnEntity(const std::string & entityName, Vector2f pos, Vector2f dir = { 0.0f,0.0f }, float velocity = 0.0f);

	int GetPlayerID() { return m_playerEntityID; }

	const std::vector<Vector2f>& GetGreenSlimePath() const { return m_greenSlimePath; }
private:
	void initilise();
	const HAPISPACE::DWORD TICKTIME{ 32 };
	ECSManager m_ecsManager;
	CollisionManager m_collision_system;
	Renderer* m_renderer;

	Tilemap m_tilemap;

	int m_playerEntityID{ -1 };

	std::vector<Vector2f> m_greenSlimePath;

	HAPISPACE::DWORD m_lastTimeTicked{0};
	float m_interpolatedTime{ 0.0f };
};

