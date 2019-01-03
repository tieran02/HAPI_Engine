#include "EntityFactory.hpp"
#include <iostream>


EntityFactory::EntityFactory()
{
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::Clear()
{
	m_entities.clear();
}

void EntityFactory::CreateEntity(const std::string& name, std::vector<std::shared_ptr<BaseComponent>> components)
{
	if (m_entities.find(name) != m_entities.end() || components.empty())
		return;

	m_entities[name] = std::make_unique<Entity>(0, name);

	//create key for the entity components
	for (auto& base_component : components)
	{
		//add component to entity key
		m_entities[name]->components[base_component->id] = base_component;
		m_entities[name]->m_key = m_entities[name]->m_key.to_ulong() + base_component->id;
	}
}

void EntityFactory::CreateEntity(const std::string& name, std::vector<std::string> components, const ComponentFactory& component_factory)
{
	CreateEntity(name, component_factory.MakeComponents(components));
}

void EntityFactory::CreateFromXML(const std::string& xml)
{
}

const Entity* EntityFactory::GetEntity(const Entity& entity) const
{
	if (m_entities.find(entity.GetName()) != m_entities.end())
	{
		return m_entities.at(entity.GetName()).get();
	}
	return nullptr;
}

std::shared_ptr<Entity> EntityFactory::Instantiate(const std::string& entityName, int id)
{
	//check if name is withing the entity map
	if (m_entities.find(entityName) == m_entities.end())
	{
		std::cout << "Can't find entity with the name '" << entityName << "' in the factory of entities\n";
		return nullptr;
	}

	auto entity = std::make_shared<Entity>(*m_entities[entityName].get());
	for (auto& component : entity->components)
	{
		component.second = entity->components[component.first]->Clone();
	}
	entity->m_id = id;
	entity->SetName(entityName);
	return entity;
	
}
