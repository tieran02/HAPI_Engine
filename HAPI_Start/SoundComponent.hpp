#pragma once
#include "Component.hpp"
#include <string>

struct SoundComponent : public Component<SoundComponent> {
	SoundComponent() {}
	std::shared_ptr<BaseComponent> Clone() const override;

private:
	std::string m_soundName;
	int m_volume{ 100 };
	bool m_playing;
	bool m_onEnable{ true };
public:
	void SetSound(const std::string& sound) { m_soundName = sound; }
	const std::string& GetSound() const { return m_soundName; }
	void SetPlaying(bool playing) { m_playing = playing; }
	bool IsPlaying() const { return m_playing; }
	void SetVolume(int volume) { m_volume = volume; }
	int GetVolume() const { return m_volume; }
	void OnEnable(bool enable) { m_onEnable = enable; }
	bool IsOnEnable() const { return m_onEnable; }
};

inline std::shared_ptr<BaseComponent> SoundComponent::Clone() const
{
	return std::make_shared<SoundComponent>(*this);
}