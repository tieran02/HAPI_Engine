#include "ECSManager.hpp"

ECSManager::~ECSManager()
{
	
}

void ECSManager::Clear()
{
	for (auto& entity : m_entities)
	{
		entity->components.clear();
	}
	m_entities.clear();
	m_systems.clear();

	m_entityFactory.Clear();
	m_componentFactory.Clear();

}

std::vector<std::shared_ptr<BaseComponent>> ECSManager::MakeComponents(std::vector<std::string> componentNames)
{
	return m_componentFactory.MakeComponents(componentNames);;
}

void ECSManager::MakeEntity(std::vector<std::string> componentNames, const std::string& name)
{
	//Get all the components from the component factory
	auto components = m_componentFactory.MakeComponents(componentNames);

	//check if the components is valid
	if(components.empty())
		return;

	//add entity to the entity factory
	m_entityFactory.CreateEntity(name, componentNames, m_componentFactory);
}

void ECSManager::MakeEntity(std::vector<std::shared_ptr<BaseComponent>> components, const std::string& name)
{
	//check if the components is valid
	if (components.empty())
		return;

	//add entity to the entity factory
	m_entityFactory.CreateEntity(name, components);
}

Entity* ECSManager::InstantiateEntity(const std::string& name)
{
	const auto entity = m_entityFactory.Instantiate(name, m_entities.size());
	if (entity != nullptr) {
		m_entities.push_back(entity);
		return m_entities.back().get();
	}
	return nullptr;
}

void ECSManager::RemoveEntity(int entityID)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if(m_entities[i]->ID() == entityID)
		{
			//Remove instance sprite from renderer if it has one
			m_renderer->RemoveInstance(m_entities[i]->ID());
			m_entities.erase(m_entities.begin() + i);
		}
	}
}

void ECSManager::UpdateSystems()
{
	for (auto& entity : m_entities)
	{
		for (auto& system : m_systems)
		{
			if (entity != nullptr && (system->GetSignature() & ~entity->GetKey()).none()) {
				system->Update((*this), *entity.get());
			}
		}
	}
}
