#include "pch.h"
#include "AudioSourceComponent.h"
#include "SceneManager.h"
#include "AudioSourceSystem.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "ResourceManager.h"

namespace Bloodforge
{
	AudioSourceComponent::AudioSourceComponent()
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->AddAudioSource(*this);
	}

	AudioSourceComponent::~AudioSourceComponent()
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->RemoveAudioSource(*this);
	}

	void AudioSourceComponent::Play()
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->StartPlayingAudioSource(*this);
	}

	void AudioSourceComponent::Pause()
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->PauseAudioSource(*this);
	}

	void AudioSourceComponent::Stop()
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->StopPlayingAudioSource(*this);
	}

	void AudioSourceComponent::SetAudio(SoundId id)
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->SetAudioOfTrack(*this, id);
	}

	void AudioSourceComponent::SetAudioGroup(AudioGroupId id)
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->AddAudioToAudioGroup(*this, id);
	}

	void AudioSourceComponent::SetVolume(float volume)
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->SetAudioTrackVolume(*this, volume);
	}
}