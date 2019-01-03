#include "CollisionManager.hpp"



CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::AddCollisionObject(const Rect& rect, CollisionObject::CollisionLayer layer, unsigned int layerMask, int id)
{
	m_collision_objects.insert(std::make_pair(id, CollisionObject(rect, layer,layerMask, id)));
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

}

int CollisionManager::IsColliding(int id)
{
	auto& collision_object = m_collision_objects.at(id);

	for (auto& other_collision_object : m_collision_objects)
	{
		if (!m_collision_objects[other_collision_object.second.ID].Active)
			return -1;

		//Check whether the objects are the same if so skip this instance
		if (collision_object.ID == other_collision_object.second.ID)
			continue;

		//check the layers
		bool bit = (other_collision_object.second.Layer & collision_object.CollidesWith) != 0;

		if (!bit)
			continue;

		//Check if the rects intersect
		if (collision_object.CollisionRectangle.Intersect(other_collision_object.second.CollisionRectangle))
		{
			return other_collision_object.second.ID;
		}
	}
	return -1;

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
