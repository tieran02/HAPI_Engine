#pragma once
#include "Rect.hpp"
#include <unordered_map>


struct CollisionObject
{
	enum CollisionLayer
	{
		None = 0,
		Player = 1,
		Enemy = 2,
		World = 4,
		Effect = 8,
		Half_Cover = 16
	};

	CollisionObject(){}
	CollisionObject(const Rect& rect, CollisionObject::CollisionLayer layer, unsigned int layerMask, int id) : CollisionRectangle(rect),Layer(layer), CollidesWith(layerMask), ID(id), LastRectangle(rect){}
	Rect CollisionRectangle;
	Rect LastRectangle;
	CollisionLayer Layer;
	unsigned int CollidesWith;
	int ID{ 0 };
	bool Active{ true };
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void AddCollisionObject(const Rect& rect, CollisionObject::CollisionLayer layer, unsigned int layerMask, int id);
	void UpdateCollisionObject(int id, const Rect& rect);
	void RemoveCollisionObject(int id);
	void SetCollisionObject(int id, bool active);
	Rect GetObjectRect(int id) const;

	void UpdateCollisions();

	int IsColliding(int id);

	int ObjectCount() const;

	std::unordered_map<int, Rect> GetUpdatedRects();
	void Clear();
private:
	std::unordered_map<int, CollisionObject> m_collision_objects;
	std::vector<int> m_markedForDeleteion;

};

