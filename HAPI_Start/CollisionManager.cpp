#include "CollisionManager.hpp"



CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::AddCollisionObject(const Rect& rect, CollisionLayer layerMask, int id)
{
	m_collision_objects.insert(std::make_pair(id, CollisionObject(rect, layerMask, id)));
}

void CollisionManager::UpdateCollisionObject(int id, const Rect& rect)
{
	if (m_collision_objects.find(id) != m_collision_objects.end()) 
	{
		m_collision_objects[id].LastRectangle = m_collision_objects[id].CollisionRectangle;
		m_collision_objects[id].CollisionRectangle = rect;
	}
}

void CollisionManager::RemoveCollisionObject(int id)
{
	if (m_collision_objects.find(id) != m_collision_objects.end())
		m_collision_objects.erase(id);
}

void CollisionManager::SetCollisionObject(int id, bool active)
{
	if (m_collision_objects.find(id) != m_collision_objects.end())
		m_collision_objects[id].Active = active;
}

Rect CollisionManager::GetObjectRect(int id) const
{
	if (m_collision_objects.find(id) != m_collision_objects.end())
		return m_collision_objects.at(id).CollisionRectangle;

	return Rect();
}

void CollisionManager::UpdateCollisions()
{
	for (auto& collision_object : m_collision_objects)
	{
		for (auto& other_collision_object : m_collision_objects)
		{
			//Check whether the objects are the same if so skip this instance
			if(collision_object.second.ID == other_collision_object.second.ID)
				continue;

			//Check if the rects intersect
			if(collision_object.second.CollisionRectangle.Intersect(other_collision_object.second.CollisionRectangle))
			{
				//set the rect back to the old one
				collision_object.second.CollisionRectangle = collision_object.second.LastRectangle;
				other_collision_object.second.CollisionRectangle = other_collision_object.second.LastRectangle;
			}
		}
	}
}

int CollisionManager::ObjectCount() const
{
	return m_collision_objects.size();
}

std::unordered_map<int, Rect> CollisionManager::GetUpdatedRects()
{
	std::unordered_map<int, Rect> rects;

	for (auto& collision_object : m_collision_objects)
	{
		rects[collision_object.second.ID] = collision_object.second.CollisionRectangle;
	}

	return rects;
}

void CollisionManager::Clear()
{
	m_collision_objects.clear();
}
