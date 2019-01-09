#include "Tilemap.hpp"
#include <HAPI_lib.h>
#include <sstream>

using namespace HAPISPACE;

Tilemap::Tilemap()
{
}


Tilemap::~Tilemap()
{
}

void Tilemap::LoadFromFile(const std::string& path, CollisionManager& collisionSystem)
{
	m_layers.clear();

	CHapiXML xml(path);
	auto node = xml.GetFirstNodeWithName("map");

	CHapiXMLAttribute attribute;
	if (!node->GetAttributeWithName("width", attribute))
		return;
	m_width = attribute.AsInt();

	if (!node->GetAttributeWithName("height", attribute))
		return;
	m_height = attribute.AsInt();

	if (!node->GetAttributeWithName("tilewidth", attribute))
		return;
	m_tileWidth = attribute.AsInt();

	if (!node->GetAttributeWithName("tileheight", attribute))
		return;
	m_tileHeight = attribute.AsInt();

	node = xml.GetFirstNodeWithName("tileset");
	if (!node->GetAttributeWithName("source", attribute))
		return;
	m_textureName = attribute.AsString();

	//get layers
	auto dataNodes = xml.GetAllNodesWithName("data");

	//resize layers
	m_layers.resize(dataNodes.size());
	for (int i = 0; i < dataNodes.size(); ++i)
	{
		std::string layerData = dataNodes[i]->GetChildren()[0]->GetValue().AsString();

		//check if the layer is the Collision layer
		CHapiXMLAttribute attribute;
		if(!dataNodes[i]->GetParent()->GetAttributeWithName("name",attribute))
			return;
		if(attribute.AsString() == "Collision")
		{
			LoadCollisionLayer(layerData,collisionSystem);
			continue;
		}

		LoadLayer(i, layerData);
	}
}

void Tilemap::Draw(Renderer& renderer)
{
	for (auto& layer : m_layers)
	{
		int x = 0;
		int y = 0;
		for (int i = 0; i < layer.size(); ++i)
		{

			if (i != 0 && i % (m_width) == 0)
			{
				//newLine
				y++;
				x = 0;
			}
				renderer.DrawTile(m_textureName, Vector2f((float)layer[i].x, (float)layer[i].y), layer[i].tileID);
			x++;
		}
	}
}

void Tilemap::LoadCollisionLayer(const std::string& layerData, CollisionManager& collisionSystem)
{
	std::istringstream is(layerData);

	std::string line;

	int x = 0;
	int y = 0;
	//get each tile value by separating the string at each comma
	while (std::getline(is, line, ','))
	{
		if (line.empty())
			continue;
		if (line.find("\n") != std::string::npos)
		{
			line.erase(line.find("\n"), 1);
		}

		//convert string to int
		int tileID = std::stoi(line) - 1;
		if (tileID >= 0)
		{
			int collisionX = x * m_tileWidth;
			int collisionY = y * m_tileHeight;
			Rect collisionRect = Rect((float)collisionX, (float)collisionX + (float)m_tileWidth, (float)collisionY, (float)collisionY + (float)m_tileHeight);

			collisionSystem.AddCollisionObject(collisionRect, CollisionObject::CollisionLayer::World,
				CollisionObject::CollisionLayer::None,
				collisionSystem.ObjectCount());
		}

		if (x < m_width - 1)
			x++;
		else
		{
			x = 0;
			y++;
		}
	}
}

void Tilemap::LoadLayer(int layerIndex, const std::string& layerData)
{
	std::istringstream is(layerData);

	std::string line;

	int x = 0;
	int y = 0;
	//get each tile value by separating the string at each comma
	while (std::getline(is, line, ','))
	{
		if(line.empty())
			continue;
		if (line.find("\n") != std::string::npos)
		{
			line.erase(line.find("\n"), 1);
		}

		//convert string to int
		int tileID = std::stoi(line)-1;
		if (tileID >= 0)
		{
			m_layers[layerIndex].emplace_back(Tile{ x* m_tileWidth,y * m_tileHeight ,tileID});
		}
		
		if (x < m_width - 1)
			x++;
		else
		{
			x = 0;
			y++;
		}
	}

}
