#pragma once
#include "Rect.hpp"
#include <vector>
#include <unordered_map>

enum class CollisionLayer {
	World,
	Player,
	Enemy
};

struct CollisionObject
{
	CollisionObject(){}
	CollisionObject(const Rect& rect, CollisionLayer layerMask, int id) : CollisionRectangle(rect), LayerMask(layerMask), ID(id), LastRectangle(rect){}
	Rect CollisionRectangle;
	Rect LastRectangle;
	CollisionLayer LayerMask;
	int ID{ 0 };
	bool Active{ true };
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void AddCollisionObject(const Rect& rect, CollisionLayer layerMask, int id);
	void UpdateCollisionObject(int id, const Rect& rect);
	void RemoveCollisionObject(int id);
	void SetCollisionObject(int id, bool active);
	Rect GetObjectRect(int id) const;

	void UpdateCollisions();

	int ObjectCount() const;

	std::unordered_map<int, Rect> GetUpdatedRects();
	void Clear();
private:
	std::unordered_map<int, CollisionObject> m_collision_objects;

};

