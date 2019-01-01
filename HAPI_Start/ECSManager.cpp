#include "ECSManager.hpp"
#include "TransformComponent.hpp"
#include "CollidableComponent.hpp"
#include "MotionComponent.hpp"

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
	m_entityPools.clear();
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
	//check if entity is in a entity pool
	if(m_entityPools.find(name) != m_entityPools.end())
	{
		const std::shared_ptr<Entity> entity = getEntityFromPool(name);
		if (entity != nullptr)
		{
			entity->SetActive(true);
			return entity.get();
		}
		return nullptr;
	}

	const auto entity = m_entityFactory.Instantiate(name, m_entities.size());
	if (entity != nullptr) {
		m_entities.push_back(entity);
		return m_entities.back().get();
	}
	return nullptr;
}

Entity* ECSManager::InstantiateEntity(const std::string& name, Vector2f pos, Vector2f dir, float velocity)
{
	//check if entity is in a entity pool
	if (m_entityPools.find(name) != m_entityPools.end())
	{
		const std::shared_ptr<Entity> entity = getEntityFromPool(name);
		if (entity != nullptr)
		{
			entity->SetActive(true);

			TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
			MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

			if (transform != nullptr)
				transform->SetPosition(pos);
			if (motion != nullptr) {
				motion->Direction = dir;
				motion->Velocity = velocity;
			}

			return entity.get();
		}
		return nullptr;
	}

	const auto entity = m_entityFactory.Instantiate(name, m_entities.size());
	if (entity != nullptr) {
		TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
		MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

		if (transform != nullptr)
			transform->SetPosition(pos);
		if (motion != nullptr) {
			motion->Direction = dir;
			motion->Velocity = velocity;
		}

		m_entities.push_back(entity);
		return m_entities.back().get();
	}
	return nullptr;
}

Entity * ECSManager::GetCollidableEntity(int id)
{
	return m_collidableEntities[id].get();;
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

			//check if the entity is a pooled item
			if (m_entityPools.find(m_entities[i]->GetName()) != m_entityPools.end())
			{
				//disable entity instead of remove them
				m_entities[i]->SetActive(false);
				return;
			}
			//else delte it from the entities list
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
		if (entity == nullptr || !entity->IsActive())
			continue;

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

void ECSManager::CreateEntityPool(const std::string& entityName, int size)
{
	//check if entity pool already exists
	if(m_entityPools.find(entityName) == m_entityPools.end())
	{
		//resize pool
		m_entityPools[entityName].resize(size);

		for (int i = 0; i < size; ++i)
		{
			const auto entity = m_entityFactory.Instantiate(entityName, m_entities.size());
			entity->SetActive(false);
			m_entityPools[entityName][i] = entity;
			//also add the pooled entites to the entity list
			m_entities.push_back(entity);
		}
	}
}

bool ECSManager::HasEntityPool(const std::string & entityName)
{
	if(m_entityPools.find(entityName) == m_entityPools.end())
		return false;

	return true;
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
			transform_component->SetPosition(Vector2f(collisonRect.second.Left, collisonRect.second.Top));
		}
	}
}

std::shared_ptr<Entity> ECSManager::getEntityFromPool(const std::string& entityName)
{
	if (m_entityPools.find(entityName) != m_entityPools.end())
	{
		//get first inactive entity
		for (auto& entity : m_entityPools[entityName])
		{
			if (!entity->IsActive())
				return entity;
		}
	}
	return nullptr;
}
