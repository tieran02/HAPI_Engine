#pragma once
#include <string>
#include <vector>
#include "Entity.hpp"
#include <unordered_map>
#include "ComponentFactory.hpp"

class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();

	void Clear();

	void CreateEntity(const std::string& name, std::vector<std::shared_ptr<BaseComponent>> components);
	void CreateEntity(const std::string& name, std::vector<std::string> components, const ComponentFactory& component_factory);
	void CreateFromXML(const std::string& xml);

	const Entity* GetEntity(const Entity& entity) const;
	std::shared_ptr<Entity> Instantiate(const std::string& entityName, int id);
private:
	std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
};

