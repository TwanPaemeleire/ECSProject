#include "pch.h"
#include "AudioSource.h"
#include "SceneManager.h"
#include "AudioSourceSystem.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "ResourceManager.h"

namespace Bloodforge
{
	AudioSource::AudioSource()
	{
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->AddAudioSource(*this);
	}

	void AudioSource::Play()
	{
		IsPlaying = true;
		IsPaused = false;
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->StartPlayingAudioSource(*this);
	}

	void AudioSource::Pause()
	{
		if (!IsPlaying) return;
		IsPaused = true;
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->PauseAudioSource(*this);
	}

	void AudioSource::Stop()
	{
		if (!IsPlaying) return;
		IsPaused = false;
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->StopPlayingAudioSource(*this);
	}

	void AudioSource::SetAudio(SoundId id)
	{
		MIX_Audio* audio = ResourceManager::GetInstance().GetAudio(id);
		if (audio == nullptr)
		{
			throw std::exception("Trying to set an audio with an id that hasn't been loaded.");
		}
		CurrentSoundId = id;
		SceneManager::GetInstance().GetActiveScene().GetSystem<AudioSourceSystem>()->SetAudioOfTrack(*this);
	}
}