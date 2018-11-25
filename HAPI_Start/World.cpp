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
	m_tilemap.LoadFromFile("Data\\Level1.xml");

	m_ecsManager.SetRenderer(m_renderer);


	//ADD ECS components
	m_ecsManager.AddComponentToFactory<TransformComponent>("TransformComponent");
	m_ecsManager.AddComponentToFactory<MotionComponent>("MotionComponent");
	m_ecsManager.AddComponentToFactory<ControllerComponent>("ControllerComponent");
	m_ecsManager.AddComponentToFactory<AnimationComponent>("AnimationComponent");
	m_ecsManager.AddComponentToFactory<SpriteComponent>("SpriteComponent");
	//ADD ECS systems
	m_ecsManager.AddSystem<ControllerSystem>();
	m_ecsManager.AddSystem<MovementSystem>();
	m_ecsManager.AddSystem<AnimationSystem>();
	m_ecsManager.AddSystem<RenderSystem>();

	//Make player Entity
	auto playerComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "ControllerComponent", "AnimationComponent", "SpriteComponent" });
	if (!playerComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)playerComponents[3].get();
		animation_component->Animations["idleRight"] = std::make_tuple("mageIdleRight",true);
		animation_component->Animations["idleLeft"] = std::make_tuple("mageIdleLeft", true);
		animation_component->Animations["walkRight"] = std::make_tuple("mageWalkRight", true);
		animation_component->Animations["walkLeft"] = std::make_tuple("mageWalkLeft", true);
		animation_component->Animations["attackRight"] = std::make_tuple("mageAttackRight", false);
		animation_component->Animations["attackLeft"] = std::make_tuple("mageAttackLeft", false);

		SpriteComponent* sprite_component = (SpriteComponent*)playerComponents[4].get();
		sprite_component->Type = SpriteType::Animated;
		sprite_component->Speed = 0.2f;
	}
	m_ecsManager.MakeEntity(playerComponents, "Player");

	//Make player Entity
	auto enemyComponents = m_ecsManager.MakeComponents({ "TransformComponent","MotionComponent", "AnimationComponent", "SpriteComponent" });
	if (!enemyComponents.empty())
	{
		AnimationComponent* animation_component = (AnimationComponent*)enemyComponents[2].get();
		animation_component->Animations["idleRight"] = std::make_tuple("mageIdleRight", true);
		animation_component->Animations["idleLeft"] = std::make_tuple("mageIdleLeft", true);
		animation_component->Animations["walkRight"] = std::make_tuple("mageWalkRight", true);
		animation_component->Animations["walkLeft"] = std::make_tuple("mageWalkLeft", true);
		animation_component->Animations["attackRight"] = std::make_tuple("mageAttackRight", false);
		animation_component->Animations["attackLeft"] = std::make_tuple("mageAttackLeft", false);

		SpriteComponent* sprite_component = (SpriteComponent*)enemyComponents[3].get();
		sprite_component->Type = SpriteType::Animated;
		sprite_component->Speed = 0.2f;
	}
	m_ecsManager.MakeEntity(enemyComponents, "Enemy");


	SpawnEntity("Player", Vector3f(0, 100, 0));
	SpawnEntity("Enemy", Vector3f(500, 100, 0));
}

void World::Unload()
{
	m_ecsManager.Clear();
}

void World::Update()
{
	//Update entity component system

	if (HAPI.GetTime() - m_lastTime > 16) {
		m_ecsManager.UpdateSystems();

		m_lastTime = HAPI.GetTime();
	}
}

void World::Render()
{
	m_tilemap.Draw(*m_renderer);
}


void World::SpawnEntity(const std::string & entityName, Vector3f pos, Vector2f dir, float velocity)
{
	Entity* entity = m_ecsManager.InstantiateEntity(entityName);

	if(entity == nullptr)
		return;

	TransformComponent* transform = (TransformComponent*)entity->GetComponent(TransformComponent::ID).get();
	MotionComponent* motion = (MotionComponent*)entity->GetComponent(MotionComponent::ID).get();

	if (transform != nullptr)
		transform->Position = pos;
	if (motion != nullptr) {
		motion->Direction = dir;
		motion->Velocity = velocity;
	}
}

void World::initilise()
{

}
