#include "World.hpp"
#include "TransformComponent.hpp"
#include "MotionComponent.hpp"
#include "Vector2.hpp"
#include "SpriteComponent.hpp"
#include "MovementSystem.hpp"
#include "RenderSystem.hpp"
#include "ControllerSystem.hpp"
#include "AISystem.hpp"
#include "AIControllerComponent.hpp"
#include "ControllerComponent.hpp"
#include "AnimationComponent.hpp"
#include "AnimationSystem.hpp"
#include "CollidableComponent.hpp"
#include "CollisionSystem.hpp"
#include "DamageComponent.hpp"
#include "DamageSystem.hpp"
#include "WeaponComponent.hpp"
#include "WeaponSystem.hpp"
#include "HealthComponent.hpp"
#include "SpawnerComponent.hpp"
#include "SpawnerSystem.hpp"

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
	m_ecsManager.SetWorld(this);
	m_ecsManager.SetCollisionSystem(&m_collision_system);


	//ADD ECS components
	m_ecsManager.AddComponentToFactory<TransformComponent>("TransformComponent");
	m_ecsManager.AddComponentToFactory<MotionComponent>("MotionComponent");
	m_ecsManager.AddComponentToFactory<ControllerComponent>("ControllerComponent");
	m_ecsManager.AddComponentToFactory<AnimationComponent>("AnimationComponent");
	m_ecsManager.AddComponentToFactory<SpriteComponent>("SpriteComponent");
	m_ecsManager.AddComponentToFactory<CollidableComponent>("CollidableComponent");
	m_ecsManager.AddComponentToFactory<DamageComponent>("DamageComponent");
	m_ecsManager.AddComponentToFactory<WeaponComponent>("WeaponComponent");
	m_ecsManager.AddComponentToFactory<HealthComponent>("HealthComponent");
	m_ecsManager.AddComponentToFactory<AIControllerComponent>("AIControllerComponent");
	m_ecsManager.AddComponentToFactory<SpawnerComponent>("SpawnerComponent");

	//ADD ECS systems
	m_ecsManager.AddSystem<SpawnerSystem>();
	m_ecsManager.AddSystem<ControllerSystem>();
	m_ecsManager.AddSystem<AISystem>();
	m_ecsManager.AddSystem<MovementSystem>();
	m_ecsManager.AddSystem<WeaponSystem>();
	m_ecsManager.AddSystem<CollisionSystem>();
	m_ecsManager.AddSystem<DamageSystem>();
	m_ecsManager.AddSystem<AnimationSystem>();
	m_ecsManager.AddSystem<RenderSystem>();

	//Make player Entity
	auto playerComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "ControllerComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "WeaponComponent", "HealthComponent" });
	if (!playerComponents.empty())
	{
		MotionComponent* motion_component = (MotionComponent*)playerComponents[1].get();
		motion_component->MovementSpeed = 10.0f;

		AnimationComponent* animation_component = (AnimationComponent*)playerComponents[3].get();
		animation_component->Animations["idleLeft"] = std::make_tuple("playerIdleLeft",true);
		animation_component->Animations["idleRight"] = std::make_tuple("playerIdleRight", true);
		animation_component->Animations["walkLeft"] = std::make_tuple("playerRunLeft", true);
		animation_component->Animations["walkRight"] = std::make_tuple("playerRunRight", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)playerComponents[5].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Player;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Enemy);


		WeaponComponent* weapon_component = (WeaponComponent*)playerComponents[6].get();
		weapon_component->EntityToFire = "Bullet";
		weapon_component->Firerate = 8;

		HealthComponent* health_component = (HealthComponent*)playerComponents[7].get();
		health_component->Health = 100.0f;
	}
	m_ecsManager.MakeEntity(playerComponents, "Player");

	//Make slime Entity
	auto slimeComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "HealthComponent", "AIControllerComponent", "WeaponComponent" });
	if (!slimeComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)slimeComponents[2].get();
		animation_component->Animations["slime"] = std::make_tuple("slimeAnimation", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)slimeComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Enemy;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Player);

		HealthComponent* health_component = (HealthComponent*)slimeComponents[5].get();
		health_component->Health = 5.0f;

		AIControllerComponent* ai_component = (AIControllerComponent*)slimeComponents[6].get();
		ai_component->Enemy = AIControllerComponent::EnemyType::GreenSlime;
		ai_component->MoveSpeed = 2.5f;
		ai_component->DetectRange = 10.0f;

		WeaponComponent* weapon_component = (WeaponComponent*)slimeComponents[7].get();
		weapon_component->EntityToFire = "GreenBullet";
		weapon_component->Firerate = 8;
	}
	m_ecsManager.MakeEntity(slimeComponents, "Slime");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Slime", 32);

	////Bullet Entity
	auto bulletComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "DamageComponent", "SpriteComponent", "CollidableComponent" });
	if (!bulletComponents.empty())
	{
		SpriteComponent* sprite_component = (SpriteComponent*)bulletComponents[3].get();
		sprite_component->SetSprite("bulletSprite");

		CollidableComponent* collidable_component = (CollidableComponent*)bulletComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Effect;
		collidable_component->CollideWith = CollidableComponent::CollisionLayer::None;
		collidable_component->isTrigger = true;


		DamageComponent* damage_component = (DamageComponent*)bulletComponents[2].get();
		damage_component->DestroyOnHit = true;
		damage_component->Damage = 10.0f;
		damage_component->EntityToSpawnOnHit = "Explosion";
	}
	m_ecsManager.MakeEntity(bulletComponents, "Bullet");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Bullet", 128);

	auto slimeBulletComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "DamageComponent", "SpriteComponent", "CollidableComponent" });
	if (!slimeBulletComponents.empty())
	{
		SpriteComponent* sprite_component = (SpriteComponent*)slimeBulletComponents[3].get();
		sprite_component->SetSprite("greenBulletSprite");

		CollidableComponent* collidable_component = (CollidableComponent*)slimeBulletComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Effect;
		collidable_component->CollideWith = CollidableComponent::CollisionLayer::None;
		collidable_component->isTrigger = true;


		DamageComponent* damage_component = (DamageComponent*)slimeBulletComponents[2].get();
		damage_component->DestroyOnHit = true;
		damage_component->Damage = 0.0f;
		damage_component->EntityToSpawnOnHit = "GreenExplosion";
	}
	m_ecsManager.MakeEntity(slimeBulletComponents, "GreenBullet");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("GreenBullet", 256);

	//Explosion Entity
	auto explosiontComponents = m_ecsManager.MakeComponents({ "TransformComponent", "SpriteComponent", "AnimationComponent"});
	if (!explosiontComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)explosiontComponents[2].get();
		animation_component->Animations["explosion"] = std::make_tuple("explosionAnimation", false);
		animation_component->Speed = 0.06f;
		animation_component->DestroyOnFinish = true;
	}
	m_ecsManager.MakeEntity(explosiontComponents, "Explosion");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Explosion", 128);

	//Green Explosion Entity
	auto greenExplosiontComponents = m_ecsManager.MakeComponents({ "TransformComponent", "SpriteComponent", "AnimationComponent" });
	if (!explosiontComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)greenExplosiontComponents[2].get();
		animation_component->Animations["explosion"] = std::make_tuple("greenExplosionAnimation", false);
		animation_component->Speed = 0.06f;
		animation_component->DestroyOnFinish = true;
	}
	m_ecsManager.MakeEntity(greenExplosiontComponents, "GreenExplosion");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("GreenExplosion", 256);

	//SlimeSpawner
	auto slimeSpawnerComponents = m_ecsManager.MakeComponents({ "TransformComponent", "SpawnerComponent" });
	if (!slimeSpawnerComponents.empty())
	{
		SpawnerComponent* spawner_component = (SpawnerComponent*)slimeSpawnerComponents[1].get();
		spawner_component->EntityToSpawn = "Slime";
		spawner_component->SpawnRate = 50;
		spawner_component->SpawnLimit = 10;
	}
	m_ecsManager.MakeEntity(slimeSpawnerComponents, "SlimeSpawner");

	m_playerEntityID = SpawnEntity("Player", Vector2f(100, 200));

	SpawnEntity("SlimeSpawner", Vector2f(100, 100));

	//Enemy Paths
	m_greenSlimePath.push_back({ 211.0f,728.0f });
	m_greenSlimePath.push_back({ 623.0f,953.0f });
	m_greenSlimePath.push_back({ 623.0f,1090.0f });
	m_greenSlimePath.push_back({ 200.0f,1090.0f });

	m_ecsManager.SetupEntities();
}

void World::Unload()
{
	m_ecsManager.Clear();
	m_collision_system.Clear();
	m_greenSlimePath.clear();
}

void World::Update()
{
	//Update entity component system
	HAPISPACE::DWORD timeSinceLastTick{ HAPI.GetTime() - m_lastTimeTicked };

	if (timeSinceLastTick >= TICKTIME)
	{
		m_ecsManager.UpdateSystems();

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


int World::SpawnEntity(const std::string & entityName, Vector2f pos, Vector2f dir, float velocity)
{
	Entity* entity = m_ecsManager.InstantiateEntity(entityName);

	if(entity == nullptr)
		return -1;

	TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
	MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

	if (transform != nullptr)
		transform->InitilisePosition(pos);
	if (motion != nullptr) {
		motion->Direction = dir;
		motion->Velocity = velocity;
	}

	return entity->ID();
}

void World::initilise()
{

}
