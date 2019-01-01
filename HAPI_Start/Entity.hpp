#pragma once
#include <cstdint>
#include <memory>
#include <bitset>
#include "Component.hpp"
#include <unordered_map>


struct Entity {
	friend class ECSManager;
	friend class EntityFactory;

	Entity(uint32_t id, std::string name = "Entity") : m_id(id), m_name(name) {}

	int ID() const { return m_id; }
	/// <summary> Get a key that fits a set of components of the entity </summary>
	/// <returns> A bitset that is used a to represent a key </returns>
	std::bitset<64> GetKey() const{ return m_key; }

	/// <summary> Set a name for this entity </summary>
	/// <param name="name"> New name of the entity </param>
	void SetName(const std::string& name) { m_name = name; }

	/// <summary> Get the name for this entity </summary>
	/// <returns> Name of the entity </returns>
	const std::string& GetName() { return m_name; }

	std::shared_ptr<BaseComponent> GetComponent(int id) const;

	bool IsActive() const { return m_active; }
	void SetActive(bool active) { m_active = active; }

private:
	uint32_t m_id;
	std::bitset<64> m_key{ 0 };
	std::unordered_map<int, std::shared_ptr<BaseComponent>> components;

	std::string m_name;
	bool m_active{true};
};

inline std::shared_ptr<BaseComponent> Entity::GetComponent(int id) const
{
	if(components.find(id) != components.end())
		return components.at(id);
	return nullptr;
}
