#include "ECS.hpp"
#include "World.hpp"
#include "UiManager.hpp"
#include "UiTextElement.hpp"
#include <algorithm>

World::World() 
{
}


World::~World()
{
}

void World::LoadLevel(Renderer* renderer)
{
	m_renderer = renderer;

	UiManager::Instance().SetRenderer(renderer);

	//load tilemap
	m_tilemap.LoadFromFile("Data\\Level1.xml", m_collision_system);

	initiliseECS();

	//Spawn entities
	m_playerEntityID = SpawnEntity("Player", Vector2f(255, 1100));

	m_objectiveEntityID = SpawnEntity("Objective", Vector2f(145.5, 1098.5));

	SpawnEntity("Wave", Vector2f());

	//Enemy Paths
	m_greenSlimePath.push_back({ 211.0f,728.0f });
	m_greenSlimePath.push_back({ 623.0f,953.0f });
	m_greenSlimePath.push_back({ 623.0f,1090.0f });
	m_greenSlimePath.push_back({ 196,1090.0f });

	m_ogrePath.push_back({ 1024,728.0f });
	m_ogrePath.push_back({ 623.0f,953.0f });
	m_ogrePath.push_back({ 623.0f,1090.0f });
	m_ogrePath.push_back({ 196,1090.0f });

	m_ecsManager.SetupEntities();

	//Add UI elements
	auto WaveText = std::make_shared<UiTextElement>();
	WaveText->SetText("Wave: 0");
	WaveText->SetFontSize(24);
	WaveText->SetPosition(Vector2f{ 0.9f,0.0f });
	UiManager::Instance().AddUIElement("WaveText", WaveText);

	auto ScoreText = std::make_shared<UiTextElement>();
	ScoreText->SetText("Score = 0");
	ScoreText->SetFontSize(16);
	ScoreText->SetPosition(Vector2f{ 0.9f,0.04f });
	UiManager::Instance().AddUIElement("ScoreText", ScoreText);

	auto WaveCountdownText = std::make_shared<UiTextElement>();
	WaveCountdownText->SetText("");
	WaveCountdownText->SetFontSize(24);
	WaveCountdownText->SetPosition(Vector2f{ 0.45f,0.0f });
	UiManager::Instance().AddUIElement("WaveCountdownText", WaveCountdownText);
}

void World::UnloadLevel()
{
	m_ecsManager.Clear();
	m_collision_system.Clear();
	m_greenSlimePath.clear();
	m_gameOver = false;
	m_highScore = 0;
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

	m_interpolatedTime = std::clamp(timeSinceLastTick / (float)TICKTIME,0.1f,1.0f);

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

void World::initiliseECS()
{
	m_ecsManager.SetRenderer(m_renderer);
	m_ecsManager.SetWorld(this);
	m_ecsManager.SetCollisionSystem(&m_collision_system);

	setupComponents();
	setuSystems();
	setupEntites();
}

void World::setupComponents()
{
	//ADD ECS components
	m_ecsManager.AddComponentToFactory<TransformComponent>("TransformComponent");
	m_ecsManager.AddComponentToFactory<MotionComponent>("MotionComponent");
	m_ecsManager.AddComponentToFactory<ControllerComponent>("ControllerComponent");
	m_ecsManager.AddComponentToFactory<AnimationComponent>("AnimationComponent");
	m_ecsManager.AddComponentToFactory<SpriteComponent>("SpriteComponent");
	m_ecsManager.AddComponentToFactory<TileComponent>("TileComponent");
	m_ecsManager.AddComponentToFactory<CollidableComponent>("CollidableComponent");
	m_ecsManager.AddComponentToFactory<DamageComponent>("DamageComponent");
	m_ecsManager.AddComponentToFactory<WeaponComponent>("WeaponComponent");
	m_ecsManager.AddComponentToFactory<HealthComponent>("HealthComponent");
	m_ecsManager.AddComponentToFactory<AIControllerComponent>("AIControllerComponent");
	m_ecsManager.AddComponentToFactory<SpawnerComponent>("SpawnerComponent");
	m_ecsManager.AddComponentToFactory<PickupComponent>("PickupComponent");
	m_ecsManager.AddComponentToFactory<WaveComponent>("WaveComponent");
	m_ecsManager.AddComponentToFactory<SoundComponent>("SoundComponent");
	m_ecsManager.AddComponentToFactory<ScoreComponent>("ScoreComponent");
}

void World::setuSystems()
{
	//ADD ECS systems
	m_ecsManager.AddSystem<WaveSystem>();
	m_ecsManager.AddSystem<SpawnerSystem>();
	m_ecsManager.AddSystem<ControllerSystem>();
	m_ecsManager.AddSystem<AISystem>();
	m_ecsManager.AddSystem<MovementSystem>();
	m_ecsManager.AddSystem<WeaponSystem>();
	m_ecsManager.AddSystem<CollisionSystem>();
	m_ecsManager.AddSystem<PickupSystem>();
	m_ecsManager.AddSystem<DamageSystem>();
	m_ecsManager.AddSystem<HealthSystem>();
	m_ecsManager.AddSystem<AnimationSystem>();
	m_ecsManager.AddSystem<SoundSystem>();
	m_ecsManager.AddSystem<RenderSystem>();
}

void World::setupEntites()
{
	//objective entity
	auto objectiveComponent = m_ecsManager.MakeComponents({ "TransformComponent", "TileComponent", "CollidableComponent", "HealthComponent", "SpriteComponent" });
	if (!objectiveComponent.empty())
	{
		TileComponent* tile_component = (TileComponent*)objectiveComponent[1].get();
		tile_component->SpriteSheet = "dungeonTilesheet";
		tile_component->Index = 334;

		CollidableComponent* collidable_component = (CollidableComponent*)objectiveComponent[2].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Objective;
		collidable_component->CollideWith = CollidableComponent::CollisionLayer::None;

		HealthComponent* health_component = (HealthComponent*)objectiveComponent[3].get();
		health_component->SetMaxHealth(2000);
	}
	m_ecsManager.MakeEntity(objectiveComponent, "Objective");

	//Make player Entity
	auto playerComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "ControllerComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "WeaponComponent", "HealthComponent" });
	if (!playerComponents.empty())
	{
		MotionComponent* motion_component = (MotionComponent*)playerComponents[1].get();
		motion_component->MovementSpeed = 10.0f;

		AnimationComponent* animation_component = (AnimationComponent*)playerComponents[3].get();
		animation_component->Animations["idleLeft"] = std::make_tuple("playerIdleLeft", true);
		animation_component->Animations["idleRight"] = std::make_tuple("playerIdleRight", true);
		animation_component->Animations["walkLeft"] = std::make_tuple("playerRunLeft", true);
		animation_component->Animations["walkRight"] = std::make_tuple("playerRunRight", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)playerComponents[5].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Player;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Enemy | CollidableComponent::CollisionLayer::Objective);


		WeaponComponent* weapon_component = (WeaponComponent*)playerComponents[6].get();
		weapon_component->EntityToFire = "Bullet";
		weapon_component->Firerate = 8;

		HealthComponent* health_component = (HealthComponent*)playerComponents[7].get();
		health_component->SetMaxHealth(100.0f);
	}
	m_ecsManager.MakeEntity(playerComponents, "Player");

	//Make boss Entity
	auto bossComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "HealthComponent", "AIControllerComponent", "WeaponComponent", "ScoreComponent" });
	if (!bossComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)bossComponents[2].get();
		animation_component->Animations["idleLeft"] = std::make_tuple("bossIdleLeft", true);
		animation_component->Animations["idleRight"] = std::make_tuple("bossIdleRight", true);
		animation_component->Animations["walkLeft"] = std::make_tuple("bossRunLeft", true);
		animation_component->Animations["walkRight"] = std::make_tuple("bossRunRight", true);
		animation_component->Speed = 0.055f;

		CollidableComponent* collidable_component = (CollidableComponent*)bossComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Enemy;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Player);

		HealthComponent* health_component = (HealthComponent*)bossComponents[5].get();
		health_component->SetMaxHealth(1000.0f);

		AIControllerComponent* ai_component = (AIControllerComponent*)bossComponents[6].get();
		ai_component->Enemy = AIControllerComponent::EnemyType::Boss;
		ai_component->MoveSpeed = 1.5f;
		ai_component->DetectRange = 20.0f;

		WeaponComponent* weapon_component = (WeaponComponent*)bossComponents[7].get();
		weapon_component->EntityToFire = "Bullet";
		weapon_component->Firerate = 8;

		ScoreComponent* score_component = (ScoreComponent*)bossComponents[8].get();
		score_component->SetScore(500);
	}
	m_ecsManager.MakeEntity(bossComponents, "Boss");
	m_ecsManager.CreateEntityPool("Boss", 2);

	//Make slime Entity
	auto slimeComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "HealthComponent", "AIControllerComponent", "WeaponComponent", "ScoreComponent" });
	if (!slimeComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)slimeComponents[2].get();
		animation_component->Animations["slime"] = std::make_tuple("slimeAnimation", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)slimeComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Enemy;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Player);

		HealthComponent* health_component = (HealthComponent*)slimeComponents[5].get();
		health_component->SetMaxHealth(5.0f);

		AIControllerComponent* ai_component = (AIControllerComponent*)slimeComponents[6].get();
		ai_component->Enemy = AIControllerComponent::EnemyType::GreenSlime;
		ai_component->MoveSpeed = 2.5f;
		ai_component->DetectRange = 10.0f;

		WeaponComponent* weapon_component = (WeaponComponent*)slimeComponents[7].get();
		weapon_component->EntityToFire = "GreenBullet";
		weapon_component->Firerate = 8;

		ScoreComponent* score_component = (ScoreComponent*)slimeComponents[8].get();
		score_component->SetScore(10);
	}
	m_ecsManager.MakeEntity(slimeComponents, "Slime");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Slime", 32);

	//Make slime Entity
	auto purpleSlimeComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "HealthComponent", "AIControllerComponent", "WeaponComponent", "ScoreComponent" });
	if (!purpleSlimeComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)purpleSlimeComponents[2].get();
		animation_component->Animations["slime"] = std::make_tuple("purpleSlimeAnimation", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)purpleSlimeComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Enemy;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Player);

		HealthComponent* health_component = (HealthComponent*)purpleSlimeComponents[5].get();
		health_component->SetMaxHealth(10.0f);

		AIControllerComponent* ai_component = (AIControllerComponent*)purpleSlimeComponents[6].get();
		ai_component->Enemy = AIControllerComponent::EnemyType::GreenSlime;
		ai_component->MoveSpeed = 2.5f;
		ai_component->DetectRange = 15.0f;

		WeaponComponent* weapon_component = (WeaponComponent*)purpleSlimeComponents[7].get();
		weapon_component->EntityToFire = "PurpleBullet";
		weapon_component->Firerate = 8;

		ScoreComponent* score_component = (ScoreComponent*)purpleSlimeComponents[8].get();
		score_component->SetScore(25);
	}
	m_ecsManager.MakeEntity(purpleSlimeComponents, "PurpleSlime");
	//Create an entity pool for the purple slimes
	m_ecsManager.CreateEntityPool("PurpleSlime", 32);

	//Make ogre Entity
	auto ogreComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent", "CollidableComponent", "HealthComponent", "AIControllerComponent", "WeaponComponent", "ScoreComponent" });
	if (!ogreComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)ogreComponents[2].get();
		animation_component->Animations["idleLeft"] = std::make_tuple("ogreIdleLeft", true);
		animation_component->Animations["idleRight"] = std::make_tuple("ogreIdleRight", true);
		animation_component->Animations["walkLeft"] = std::make_tuple("ogreRunLeft", true);
		animation_component->Animations["walkRight"] = std::make_tuple("ogreRunRight", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)ogreComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Enemy;
		collidable_component->CollideWith = (CollidableComponent::CollisionLayer::World | CollidableComponent::CollisionLayer::Player);

		HealthComponent* health_component = (HealthComponent*)ogreComponents[5].get();
		health_component->SetMaxHealth(50.0f);

		AIControllerComponent* ai_component = (AIControllerComponent*)ogreComponents[6].get();
		ai_component->Enemy = AIControllerComponent::EnemyType::Ogre;
		ai_component->MoveSpeed = 2.0f;
		ai_component->DetectRange = 5.0f;

		WeaponComponent* weapon_component = (WeaponComponent*)ogreComponents[7].get();
		weapon_component->EntityToFire = "Bullet";
		weapon_component->Firerate = 25;

		ScoreComponent* score_component = (ScoreComponent*)ogreComponents[8].get();
		score_component->SetScore(50);
	}
	m_ecsManager.MakeEntity(ogreComponents, "Ogre");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Ogre", 32);

	////Bullet Entity
	auto bulletComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "DamageComponent", "SpriteComponent", "CollidableComponent", "SoundComponent" });
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
		damage_component->Damage = 7.5f;
		damage_component->EntityToSpawnOnHit = "Explosion";

		SoundComponent* sound_component = (SoundComponent*)bulletComponents[5].get();
		sound_component->SetSound("gun.wav");
		sound_component->SetVolume(10);
	}
	m_ecsManager.MakeEntity(bulletComponents, "Bullet");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("Bullet", 128);

	auto slimeBulletComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "DamageComponent", "SpriteComponent", "CollidableComponent", "SoundComponent" });
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
		damage_component->Damage = 0.5f;
		damage_component->EntityToSpawnOnHit = "GreenExplosion";

		SoundComponent* sound_component = (SoundComponent*)slimeBulletComponents[5].get();
		sound_component->SetSound("slime.wav");
		sound_component->SetVolume(5);
	}
	m_ecsManager.MakeEntity(slimeBulletComponents, "GreenBullet");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("GreenBullet", 256);

	auto purpleBulletComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "DamageComponent", "SpriteComponent", "CollidableComponent", "SoundComponent" });
	if (!purpleBulletComponents.empty())
	{
		SpriteComponent* sprite_component = (SpriteComponent*)purpleBulletComponents[3].get();
		sprite_component->SetSprite("purpleBulletSprite");

		CollidableComponent* collidable_component = (CollidableComponent*)purpleBulletComponents[4].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Effect;
		collidable_component->CollideWith = CollidableComponent::CollisionLayer::None;
		collidable_component->isTrigger = true;


		DamageComponent* damage_component = (DamageComponent*)purpleBulletComponents[2].get();
		damage_component->DestroyOnHit = true;
		damage_component->Damage = 2.0f;
		damage_component->EntityToSpawnOnHit = "PurpleExplosion";

		SoundComponent* sound_component = (SoundComponent*)purpleBulletComponents[5].get();
		sound_component->SetSound("slime.wav");
		sound_component->SetVolume(5);
	}
	m_ecsManager.MakeEntity(purpleBulletComponents, "PurpleBullet");
	//Create an entity pool for the bullets
	m_ecsManager.CreateEntityPool("PurpleBullet", 256);

	//Explosion Entity
	auto explosiontComponents = m_ecsManager.MakeComponents({ "TransformComponent", "SpriteComponent", "AnimationComponent" });
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
	//Create an entity pool for the explosions
	m_ecsManager.CreateEntityPool("GreenExplosion", 256);

	//Green Explosion Entity
	auto purpleExplosiontComponents = m_ecsManager.MakeComponents({ "TransformComponent", "SpriteComponent", "AnimationComponent" });
	if (!purpleExplosiontComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)purpleExplosiontComponents[2].get();
		animation_component->Animations["explosion"] = std::make_tuple("purpleExplosionAnimation", false);
		animation_component->Speed = 0.06f;
		animation_component->DestroyOnFinish = true;
	}
	m_ecsManager.MakeEntity(purpleExplosiontComponents, "PurpleExplosion");
	//Create an entity pool for the explosions
	m_ecsManager.CreateEntityPool("PurpleExplosion", 128);

	//Health pickup Entity
	auto healthPickupComponents = m_ecsManager.MakeComponents({ "TransformComponent", "SpriteComponent", "AnimationComponent", "CollidableComponent", "PickupComponent", "SoundComponent" });
	if (!healthPickupComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)healthPickupComponents[2].get();
		animation_component->Animations["healthPickup"] = std::make_tuple("healthPickup", true);
		animation_component->Speed = 0.075f;

		CollidableComponent* collidable_component = (CollidableComponent*)healthPickupComponents[3].get();
		collidable_component->Layer = CollidableComponent::CollisionLayer::Pickup;
		collidable_component->CollideWith = CollidableComponent::CollisionLayer::Player;

		PickupComponent* pickup_component = (PickupComponent*)healthPickupComponents[4].get();
		pickup_component->SetType(PickupComponent::PickupType::Health);
		pickup_component->SetAmount(50.0f);


		SoundComponent* sound_component = (SoundComponent*)healthPickupComponents[5].get();
		sound_component->SetSound("pickup.wav");
		sound_component->SetVolume(10);
		sound_component->OnEnable(false);
	}
	m_ecsManager.MakeEntity(healthPickupComponents, "HealthPickup");
	m_ecsManager.CreateEntityPool("HealthPickup", 32);

	//Wave
	auto waveComponents = m_ecsManager.MakeComponents({ "TransformComponent", "WaveComponent" });
	if (!waveComponents.empty())
	{
		WaveComponent* wave_component = (WaveComponent*)waveComponents[1].get();

		WaveData wave1Slimes;
		wave1Slimes.Wave = 1;
		wave1Slimes.EnemyToSpawn = "Slime";
		wave1Slimes.AmountToSpawn = 10;
		wave1Slimes.SpawnRate = 50;
		wave1Slimes.SpawnPos = Vector2f(208, 100);
		wave_component->AddWave(wave1Slimes);

		//Wave 2
		WaveData wave2Slimes;
		wave2Slimes.Wave = 2;
		wave2Slimes.EnemyToSpawn = "Slime";
		wave2Slimes.AmountToSpawn = 15;
		wave2Slimes.SpawnRate = 50;
		wave2Slimes.SpawnPos = Vector2f(208, 100);
		wave_component->AddWave(wave2Slimes);

		WaveData wave2Ogres;
		wave2Ogres.Wave = 2;
		wave2Ogres.EnemyToSpawn = "Ogre";
		wave2Ogres.AmountToSpawn = 2;
		wave2Ogres.SpawnRate = 200;
		wave2Ogres.SpawnPos = Vector2f(1041, 100);
		wave_component->AddWave(wave2Ogres);

		//Wave 3
		WaveData wave3Slimes;
		wave3Slimes.Wave = 3;
		wave3Slimes.EnemyToSpawn = "Slime";
		wave3Slimes.AmountToSpawn = 18;
		wave3Slimes.SpawnRate = 50;
		wave3Slimes.SpawnPos = Vector2f(208, 100);
		wave_component->AddWave(wave3Slimes);

		WaveData wave3Ogres;
		wave3Ogres.Wave = 3;
		wave3Ogres.EnemyToSpawn = "Ogre";
		wave3Ogres.AmountToSpawn = 5;
		wave3Ogres.SpawnRate = 200;
		wave3Ogres.SpawnPos = Vector2f(1041, 100);
		wave_component->AddWave(wave3Ogres);

		//Wave 4
		WaveData wave4Slimes;
		wave4Slimes.Wave = 4;
		wave4Slimes.EnemyToSpawn = "PurpleSlime";
		wave4Slimes.AmountToSpawn = 8;
		wave4Slimes.SpawnRate = 50;
		wave4Slimes.SpawnPos = Vector2f(208, 100);
		wave_component->AddWave(wave4Slimes);

		WaveData wave4Ogres;
		wave4Ogres.Wave = 4;
		wave4Ogres.EnemyToSpawn = "Ogre";
		wave4Ogres.AmountToSpawn = 5;
		wave4Ogres.SpawnRate = 200;
		wave4Ogres.SpawnPos = Vector2f(1041, 100);
		wave_component->AddWave(wave4Ogres);

		//Wave 5
		WaveData wave5Slimes;
		wave5Slimes.Wave = 5;
		wave5Slimes.EnemyToSpawn = "PurpleSlime";
		wave5Slimes.AmountToSpawn = 16;
		wave5Slimes.SpawnRate = 50;
		wave5Slimes.SpawnPos = Vector2f(208, 100);
		wave_component->AddWave(wave5Slimes);

		WaveData wave5Ogres;
		wave5Ogres.Wave = 5;
		wave5Ogres.EnemyToSpawn = "Ogre";
		wave5Ogres.AmountToSpawn = 8;
		wave5Ogres.SpawnRate = 200;
		wave5Ogres.SpawnPos = Vector2f(1041, 100);
		wave_component->AddWave(wave5Ogres);

		//Wave 6
		WaveData wave6Boss;
		wave6Boss.Wave = 6;
		wave6Boss.EnemyToSpawn = "Boss";
		wave6Boss.AmountToSpawn = 1;
		wave6Boss.SpawnRate = 50;
		wave6Boss.SpawnPos = Vector2f(624, 100);
		wave_component->AddWave(wave6Boss);

	}
	m_ecsManager.MakeEntity(waveComponents, "Wave");
}
