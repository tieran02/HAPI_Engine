#pragma once
#include <vector>
#include "Renderer.hpp"
#include "CollisionManager.hpp"

class Tilemap
{
public:
	Tilemap();
	~Tilemap();

	void LoadFromFile(const std::string& path, CollisionManager& collisionSystem);
	void Draw(Renderer& renderer);
private:
	struct Tile
	{
		int x, y, tileID;
	};
	std::vector<std::vector<Tile>> m_layers;
	std::string m_textureName;
	int m_width, m_height, m_tileWidth, m_tileHeight;
	std::vector<Rect> m_collisionTiles;

	void LoadCollisionLayer(const std::string& layerData,CollisionManager& collisionSystem);
	void LoadLayer(int layerIndex, const std::string& layerData);
};

