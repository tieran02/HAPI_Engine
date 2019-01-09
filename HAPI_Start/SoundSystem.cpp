#include "SoundSystem.hpp"
#include "TransformComponent.hpp"
#include "SoundComponent.hpp"
#include "Utilities.hpp"
#include "ECSManager.hpp"
#include "World.hpp"


SoundSystem::SoundSystem() : System(TransformComponent::ID | SoundComponent::ID)
{
}


SoundSystem::~SoundSystem()
{
}

void SoundSystem::Setup(ECSManager& ecsManager, Entity& entity)
{
}

void SoundSystem::OnEnable(ECSManager& ecsManager, Entity& entity)
{
	SoundComponent* sound_component = (SoundComponent*)entity.GetComponent(SoundComponent::ID).get();
	if (sound_component->IsOnEnable())
		sound_component->SetPlaying(true);
	else
		sound_component->SetPlaying(false);
}

void SoundSystem::OnDisable(ECSManager& ecsManager, Entity& entity)
{
	SoundComponent* sound_component = (SoundComponent*)entity.GetComponent(SoundComponent::ID).get();
	if (sound_component->IsOnEnable())
		sound_component->SetPlaying(false);
	else
		sound_component->SetPlaying(true);
}

void SoundSystem::Update(ECSManager& ecsManager, Entity& entity)
{
	TransformComponent* transform_component = (TransformComponent*)entity.GetComponent(TransformComponent::ID).get();
	SoundComponent* sound_component = (SoundComponent*)entity.GetComponent(SoundComponent::ID).get();

	if (sound_component->IsPlaying()) {
		//Change volume depending on the distance of the player
		int playerID = ecsManager.GetWorld()->GetPlayerID();
		Entity* playerEntity = ecsManager.GetEntity(playerID);
		float distance = 100.0f;
		if(playerEntity != nullptr)
		{
			Vector2f playerPos = ((TransformComponent*)playerEntity->GetComponent(TransformComponent::ID).get())->GetPostion();
			distance = (transform_component->GetPostion() - playerPos).Magnitude();
		}

		float volume = (float)sound_component->GetVolume() / distance;

		HAPISPACE::HAPI_TSoundOptions options;
		options.volume = volume;
		Utilities::PlaySound(sound_component->GetSound(), options);
	}

	sound_component->SetPlaying(false);
}
