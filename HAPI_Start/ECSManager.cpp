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
			SetEntityActive(entity->ID(), true);
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
			SetEntityActive(entity->ID(), true);

			TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
			MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

			if (transform != nullptr)
				transform->InitilisePosition(pos);
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
			transform->InitilisePosition(pos);
		if (motion != nullptr) {
			motion->Direction = dir;
			motion->Velocity = velocity;
		}

		m_entities.push_back(entity);
		return m_entities.back().get();
	}
	return nullptr;
}

Entity* ECSManager::InstantiateEntity(std::vector<std::shared_ptr<BaseComponent>> components, const std::string& name,
	Vector2f pos, Vector2f dir, float velocity)
{
	auto entity = std::make_shared<Entity>(m_entities.size(), name);

	//create key for the entity components
	for (auto& base_component : components)
	{
		//add component to entity key
		entity->components[base_component->id] = base_component;
		entity->m_key = entity->m_key.to_ulong() + base_component->id;
	}

	if (entity != nullptr) {
		TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
		MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

		if (transform != nullptr)
			transform->InitilisePosition(pos);
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
	return m_collidableEntities[id].get();
}

Entity * ECSManager::GetEntity(int id)
{
	return m_entities[id].get();
}

void ECSManager::RemoveEntity(int entityID)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if(m_entities[i]->ID() == entityID)
		{
			//TODO: disable collision object associated with this object
			//Remove instance sprite from renderer if it has one
			m_renderer->RemoveInstance(m_entities[i]->ID());

			//check if the entity is a pooled item
			if (m_entityPools.find(m_entities[i]->GetName()) != m_entityPools.end())
			{
				//disable entity instead of remove them
				SetEntityActive(entityID, false);
				return;
			}
			//else delte it from the entities list
			m_entities.erase(m_entities.begin() + i);
			return;
		}
	}
}

void ECSManager::SetEntityActive(int id, bool active)
{
	Entity* entity = m_entities[id].get();
	if (entity != nullptr)
	{
		if (!active && entity->IsActive())
		{
			entity->m_active = false;
			m_renderer->RemoveInstance(id);

			//On disable
			for (auto& system : m_systems)
			{
				if (entity != nullptr && (system->GetSignature() & ~entity->GetKey()).none()) {
					system->OnDisable((*this), *entity);
				}
			}
		}
		else if(!entity->IsActive())
		{
			entity->m_active = true;

			//On disable
			for (auto& system : m_systems)
			{
				if (entity != nullptr && (system->GetSignature() & ~entity->GetKey()).none()) {
					system->OnEnable((*this), *entity);
				}
			}
		}
	}
}

void ECSManager::SetupEntities()
{
	//Setup Entites
	for (auto& entity : m_entities)
	{
		SetupEntity(entity.get());
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

void ECSManager::AddEntityToCollisionMap(int collisonID, int entityID, CollidableComponent::CollisionLayer layer, unsigned int collidesWith)
{
	for (auto& entity : m_entities)
	{
		if(entity->ID() == entityID)
		{
			m_collidableEntities[collisonID] = m_entities[entityID];
			CollidableComponent* collidable_component = (CollidableComponent*)m_collidableEntities[collisonID]->GetComponent(CollidableComponent::ID).get();
			collidable_component->Layer = layer;
			collidable_component->CollideWith = collidesWith;
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
			entity->m_active = false;
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

bool ECSManager::IsCollidable(int collidableID)
{
	if (m_collidableEntities.find(collidableID) != m_collidableEntities.end())
		return true;
	return false;
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
			Vector2f center = collisonRect.second.Center();
			transform_component->SetPosition(center);
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

void ECSManager::SetupEntity(Entity* entity)
{
	for (auto& system : m_systems)
	{
		if (entity != nullptr && (system->GetSignature() & ~entity->GetKey()).none()) {
			system->Setup((*this), *entity);
		}
	}
}
