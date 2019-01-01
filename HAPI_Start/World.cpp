#include "World.hpp"
#include "TransformComponent.hpp"
#include "MotionComponent.hpp"
#include "Vector2.hpp"
#include "SpriteComponent.hpp"
#include "MovementSystem.hpp"
#include "RenderSystem.hpp"
#include "ControllerSystem.hpp"
#include "ControllerComponent.hpp"
#include "AnimationComponent.hpp"
#include "AnimationSystem.hpp"
#include "CollidableComponent.hpp"
#include "CollisionSystem.hpp"
#include "DamageComponent.hpp"
#include "DamageSystem.hpp"

World::World() 
{
}


World::~World()
{
}

void World::Load(Renderer* renderer)
{
	m_renderer = renderer;

	//load tilemap
	m_tilemap.LoadFromFile("Data\\Level1.xml", m_collision_system);

	m_ecsManager.SetRenderer(m_renderer);
	m_ecsManager.SetCollisionSystem(&m_collision_system);


	//ADD ECS components
	m_ecsManager.AddComponentToFactory<TransformComponent>("TransformComponent");
	m_ecsManager.AddComponentToFactory<MotionComponent>("MotionComponent");
	m_ecsManager.AddComponentToFactory<ControllerComponent>("ControllerComponent");
	m_ecsManager.AddComponentToFactory<AnimationComponent>("AnimationComponent");
	m_ecsManager.AddComponentToFactory<SpriteComponent>("SpriteComponent");
	m_ecsManager.AddComponentToFactory<CollidableComponent>("CollidableComponent");
	m_ecsManager.AddComponentToFactory<DamageComponent>("DamageComponent");
	//ADD ECS systems
	m_ecsManager.AddSystem<ControllerSystem>();
	m_ecsManager.AddSystem<MovementSystem>();
	m_ecsManager.AddSystem<CollisionSystem>();
	m_ecsManager.AddSystem<DamageSystem>();
	m_ecsManager.AddSystem<AnimationSystem>();
	m_ecsManager.AddSystem<RenderSystem>();

	//Make player Entity
	auto playerComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "ControllerComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent" });
	if (!playerComponents.empty())
	{
		TransformComponent* transform_component = (TransformComponent*)playerComponents[0].get();
		transform_component->Rotation = 0.785398f;

		AnimationComponent* animation_component = (AnimationComponent*)playerComponents[3].get();
		animation_component->Animations["idle"] = std::make_tuple("rifleIdle",true);
		animation_component->Animations["walk"] = std::make_tuple("rifleMove", true);
		animation_component->Animations["attack"] = std::make_tuple("rifleAttack", false);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)playerComponents[5].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Player;
	}
	m_ecsManager.MakeEntity(playerComponents, "Player");

	//Make player Entity
	auto enemyComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent" });
	if (!enemyComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)enemyComponents[2].get();
		animation_component->Animations["idle"] = std::make_tuple("testAnimation", true);
		animation_component->Animations["walk"] = std::make_tuple("rifleMove", true);
		animation_component->Animations["attack"] = std::make_tuple("rifleAttack", true);

		CollidableComponent* collidable_component = (CollidableComponent*)enemyComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Enemy;
	}
	m_ecsManager.MakeEntity(enemyComponents, "Enemy");

	//Bullet Entity
	auto bulletComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "DamageComponent", "SpriteComponent", "CollidableComponent" });
	if (!bulletComponents.empty())
	{
		SpriteComponent* sprite_component = (SpriteComponent*)bulletComponents[3].get();
		sprite_component->SetSprite("bulletSprite");

		CollidableComponent* collidable_component = (CollidableComponent*)bulletComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Effect;
		collidable_component->CollideMask = CollidableComponent::CollisionLayer::World;

		DamageComponent* damage_component = (DamageComponent*)bulletComponents[2].get();
		damage_component->DestroyOnHit = true;
	}
	m_ecsManager.MakeEntity(bulletComponents, "Bullet");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Bullet", 128);

	SpawnEntity("Player", Vector2f(100, 200));
	//SpawnEntity("Enemy", Vector2f(500, 100));

	SpawnEntity("Bullet", Vector2f(100, 400), Vector2f(1,0) , 10.0f);

}

void World::Unload()
{
	m_ecsManager.Clear();
	m_collision_system.Clear();
}

void World::Update()
{
	//Update entity component system
	HAPISPACE::DWORD timeSinceLastTick{ HAPI.GetTime() - m_lastTimeTicked };

	if (timeSinceLastTick >= TICKTIME)
	{
		m_ecsManager.UpdateSystems();
		//m_collision_system.UpdateCollisions();
		//m_ecsManager.UpdateCollisionPositions();

		m_lastTimeTicked = HAPI.GetTime();
		timeSinceLastTick = 0;
	}

	m_interpolatedTime = timeSinceLastTick / (float)TICKTIME;
}

void World::Render()
{
	//Update renderer time for interpolation between simulation and rendering

	m_tilemap.Draw(*m_renderer);

	m_renderer->DrawInstancedSprites(m_interpolatedTime);
}


void World::SpawnEntity(const std::string & entityName, Vector2f pos, Vector2f dir, float velocity)
{
	Entity* entity = m_ecsManager.InstantiateEntity(entityName);

	if(entity == nullptr)
		return;

	TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
	MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

	if (transform != nullptr)
		transform->InitilisePosition(pos);
	if (motion != nullptr) {
		motion->Direction = dir;
		motion->Velocity = velocity;
	}
}

void World::DestroyFirstEntityByName(const std::string& entityName)
{
	m_ecsManager.RemoveEntityByName(entityName);
}

void World::initilise()
{

}
