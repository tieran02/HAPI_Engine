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
	void LoadLevel(Renderer* renderer);
	void UnloadLevel();

	void Update();
	void Render();
	int SpawnEntity(const std::string & entityName, Vector2f pos, Vector2f dir = { 0.0f,0.0f }, float velocity = 0.0f);

	int GetPlayerID() { return m_playerEntityID; }
	int GetBaseID() { return m_objectiveEntityID; }

	const std::vector<Vector2f>& GetGreenSlimePath() const { return m_greenSlimePath; }
	const std::vector<Vector2f>& GetOgrePath() const { return m_ogrePath; }

	void SetEnemyCount(int count) { m_currentEnimiesAlive = count; }
	int GetEnemyCount() const { return m_currentEnimiesAlive; }
	int GetHighScore() const { return m_highScore; }
	void AddScore(int amount) { m_highScore += amount; }

	bool IsGameOver()const { return m_gameOver; }
	void GameOver() { m_gameOver = true; }
private:
	void initiliseECS();
	void setupComponents();
	void setuSystems();
	void setupEntites();

	const HAPISPACE::DWORD TICKTIME{ 32 };
	ECSManager m_ecsManager;
	CollisionManager m_collision_system;
	Renderer* m_renderer;

	Tilemap m_tilemap;

	int m_playerEntityID{ -1 };
	int m_objectiveEntityID{ -1 };
	int m_currentEnimiesAlive{ 0 };
	int m_highScore{ 0 };
	bool m_gameOver{ false };

	std::vector<Vector2f> m_greenSlimePath;
	std::vector<Vector2f> m_ogrePath;

	HAPISPACE::DWORD m_lastTimeTicked{0};
	float m_interpolatedTime{ 0.0f };
};

