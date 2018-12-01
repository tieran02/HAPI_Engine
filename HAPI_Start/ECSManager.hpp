#pragma once
#include <memory>
#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentFactory.hpp"
#include "System.hpp"
#include "EntityFactory.hpp"
#include "Renderer.hpp"
#include "CollisionManager.hpp"


class ECSManager {
public:
	ECSManager() = default;
	~ECSManager();

	//Remove ALL Sytems,Components and Entities
	void Clear();

	void SetRenderer(Renderer* renderer) { m_renderer = renderer; }
	Renderer* GetRenderer() const { return m_renderer; }
	void SetCollisionSystem(CollisionManager* collision_system) { m_collision_system = collision_system; }
	CollisionManager* GetCollisionSystem() const { return m_collision_system; }

	//Add a component to the component factory
	template <class T>
	void AddComponentToFactory(const std::string& name);
	//Add a system to the ECS
	template <class T>
	void AddSystem();

	//Makes new components from a list of component names
	std::vector<std::shared_ptr<BaseComponent>> MakeComponents(std::vector<std::string> componentNames);
	//Make an Entity from a list of component names and add the entity to the entity factory.
	void MakeEntity(std::vector<std::string> componentNames, const std::string& name);
	//Make an Entity from a pre-made components and add the entity to the entity factory.
	void MakeEntity(std::vector<std::shared_ptr<BaseComponent>> components, const std::string& name);
	//Instantiate entity (Spawn entity)
	Entity* InstantiateEntity(const std::string& name);
	//TODO: Load entities from xml
	void LoadEntitiesFromXML(const std::string& path);
	//Remove entity from the ECS
	void RemoveEntity(int entityID);
	//Remove entity by name (ONLY the first entity with that name will be removed)
	void RemoveEntityByName(const std::string& name);
	//Update all systems
	void UpdateSystems();

	//Add entity to the collision map
	void AddEntityToCollisionMap(int collisonID, int entityID);

	void UpdateCollisionPositions();
	
private:
	ComponentFactory m_componentFactory;
	EntityFactory m_entityFactory;

	std::vector<std::shared_ptr<Entity>> m_entities;
	//Seperate unoreded map for colliable entites with the collidable id as the key
	std::unordered_map<int, std::shared_ptr<Entity>> m_collidableEntities;
	std::vector<std::unique_ptr<System>> m_systems;

	//Pointer to the render for systems to access
	Renderer* m_renderer;
	CollisionManager* m_collision_system;


};

template <class T>
void ECSManager::AddComponentToFactory(const std::string& name)
{
	m_componentFactory.AddComponent<T>(name);
}

template <class T>
void ECSManager::AddSystem()
{
	m_systems.emplace_back(std::make_unique<T>());
}
