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

void CollisionManager::UpdateCollisionObject(int id, const Rect& rect, CollisionObject::CollisionLayer layer, unsigned int layerMask)
{
	if (m_collision_objects.find(id) != m_collision_objects.end()) 
	{
		m_collision_objects[id].LastRectangle = m_collision_objects[id].CollisionRectangle;
		m_collision_objects[id].CollisionRectangle = rect;
		m_collision_objects[id].Layer = layer;
		m_collision_objects[id].CollidesWith = layerMask;
	}
}

void CollisionManager::SetCollisionObjectActive(int id, bool active)
{
	if (m_collision_objects.find(id) != m_collision_objects.end())
	{
		if(m_collision_objects[id].Active != active)
			m_collision_objects[id].Active = active;
	}
}

Rect CollisionManager::GetObjectRect(int id) const
{
	if (m_collision_objects.find(id) != m_collision_objects.end())
		return m_collision_objects.at(id).CollisionRectangle;

	return Rect();
}

int CollisionManager::IsColliding(int id)
{
	if (id < 0)
		return -1;

	auto& collision_object = m_collision_objects.at(id);
	//check if the entity is active
	if (!collision_object.Active)
		return -1;

	for (auto& other_collision_object : m_collision_objects)
	{
		if (!m_collision_objects[other_collision_object.second.ID].Active)
			continue;

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
	return (int)m_collision_objects.size();
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
