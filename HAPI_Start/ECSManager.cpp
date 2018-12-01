#include "ECSManager.hpp"
#include "TransformComponent.hpp"
#include "CollidableComponent.hpp"

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
	m_collidableEntities.clear();
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
			//check if the entity is also in the 
			CollidableComponent* collidable_component = (CollidableComponent*)m_entities[i]->GetComponent(CollidableComponent::ID).get();
			if (collidable_component != nullptr) {
				if (m_collidableEntities.find(collidable_component->CollisionID) != m_collidableEntities.end())
					m_collidableEntities.erase(collidable_component->CollisionID);
			}
			//Remove instance sprite from renderer if it has one
			m_renderer->RemoveInstance(m_entities[i]->ID());
			m_entities.erase(m_entities.begin() + i);
			return;
		}
	}
}

void ECSManager::RemoveEntityByName(const std::string& name)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if(m_entities[i]->GetName() == name)
		{
			//check if the entity is also in the 
			CollidableComponent* collidable_component = (CollidableComponent*)m_entities[i]->GetComponent(CollidableComponent::ID).get();
			if (collidable_component != nullptr) {
				if (m_collidableEntities.find(collidable_component->CollisionID) != m_collidableEntities.end())
					m_collidableEntities.erase(collidable_component->CollisionID);
			}
			//Remove instance sprite from renderer if it has one
			m_renderer->RemoveInstance(m_entities[i]->ID());
			m_entities.erase(m_entities.begin() + i);
			return;
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

void ECSManager::AddEntityToCollisionMap(int collisonID, int entityID)
{
	for (auto& entity : m_entities)
	{
		if(entity->ID() == entityID)
		{
			m_collidableEntities[collisonID] = m_entities[entityID];
			return;
		}
	}
}

void ECSManager::UpdateCollisionPositions()
{
	for (auto& collisonRect : GetCollisionSystem()->GetUpdatedRects())
	{
		if(m_collidableEntities.find(collisonRect.first) == m_collidableEntities.end())
			continue;

		TransformComponent* transform_component = (TransformComponent*)m_collidableEntities[collisonRect.first]->GetComponent(TransformComponent::ID).get();

		if(transform_component != nullptr)
		{
			Vector2i center = collisonRect.second.Center();
			transform_component->Position = Vector2f(collisonRect.second.Left, collisonRect.second.Top);
		}
	}
}
