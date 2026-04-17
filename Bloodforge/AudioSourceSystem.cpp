#include "pch.h"
#include "AudioSourceSystem.h"
#include "ResourceManager.h"
#include "AudioSourceComponent.h"
#include <SDL3_mixer/SDL_mixer.h>

namespace Bloodforge
{
	void AudioSourceSystem::OnStart()
	{
		m_Mixer = ResourceManager::GetInstance().GetMixer();
	}

	void AudioSourceSystem::SetAudioOfTrack(AudioSourceComponent& audioSource)
	{
		MIX_SetTrackAudio(m_AllMixTracks[audioSource.TrackIndex], ResourceManager::GetInstance().GetAudio(audioSource.CurrentSoundId));
	}

	void AudioSourceSystem::StartPlayingAudioSource(AudioSourceComponent& audioSource)
	{
		MIX_PlayTrack(m_AllMixTracks[audioSource.TrackIndex], 0);
	}

	void AudioSourceSystem::StopPlayingAudioSource(AudioSourceComponent& audioSource)
	{
		MIX_StopTrack(m_AllMixTracks[audioSource.TrackIndex], 0);
	}

	void AudioSourceSystem::PauseAudioSource(AudioSourceComponent& audioSource)
	{
		MIX_PauseTrack(m_AllMixTracks[audioSource.TrackIndex]);
	}

	void AudioSourceSystem::ResumeAudioSource(AudioSourceComponent& audioSource)
	{
		MIX_ResumeTrack(m_AllMixTracks[audioSource.TrackIndex]);
	}

	void AudioSourceSystem::AddAudioSource(AudioSourceComponent& audioSource)
	{
		audioSource.TrackIndex = GetAvailableMixTrackIndex();
	}

	void AudioSourceSystem::RemoveAudioSource(AudioSourceComponent& audioSource)
	{
		audioSource.Stop();
		m_FreeIndices.emplace_back(audioSource.TrackIndex);
	}

	int AudioSourceSystem::GetAvailableMixTrackIndex()
	{
		if (m_FreeIndices.size() == 0)
		{
			if(m_Mixer == nullptr) m_Mixer = ResourceManager::GetInstance().GetMixer();
			MIX_Track* newTrack = MIX_CreateTrack(m_Mixer);
			m_AllMixTracks.emplace_back(newTrack);
			return static_cast<int>(m_AllMixTracks.size()) - 1;
		}
		int trackIdx = m_FreeIndices[m_FreeIndices.size() - 1];
		m_FreeIndices.erase(m_FreeIndices.end() - 1, m_FreeIndices.end());
		return trackIdx;
	}
}