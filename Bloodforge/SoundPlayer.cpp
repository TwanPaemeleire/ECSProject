#include "pch.h"
#include "SoundPlayer.h"
#include <string>
#include <memory>
#include <SDL3_mixer/SDL_mixer.h>

namespace Bloodforge
{
	SoundPlayer::~SoundPlayer()
	{
		MIX_Quit();
	}

	void SoundPlayer::RequestLoadSound(const std::string& path, SoundId id)
	{
		if (m_LoadedAudio.contains(id))
		{
			throw std::exception("Trying to add a sound with an id that is already loaded in.");
		}
		m_LoadedAudio.insert({ id, MIX_LoadAudio(m_Mixer, path.c_str(), false) });
	}

	void SoundPlayer::RequestPlaySound(SoundId id, const float)
	{
		if (!m_LoadedAudio.contains(id))
		{
			throw std::exception("Trying to play a sound with an id that hasn't been loaded yet.");
		}
		MIX_Track* track = GetAvailableMixTrack();
		MIX_Audio* audio = m_LoadedAudio[id];
		MIX_SetTrackAudio(track, audio);
		MIX_PlayTrack(track, 0);
	}

	void SoundPlayer::RequestStopAllSound(int fadeOutTimeInMs)
	{
		MIX_StopAllTracks(m_Mixer, fadeOutTimeInMs);
		m_FreeMixTracks.clear();
		for (MIX_Track* track : m_AllMixTracks)
		{
			m_FreeMixTracks.emplace_back(track);
		}
	}

	SoundPlayer::SoundPlayer()
	{
		MIX_Init();
		m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	}

	MIX_Track* SoundPlayer::GetAvailableMixTrack() // This currently gets ran whenever you try to play an audio, could be nice to find a way to make this function run occasionally on another thread
	{
		if (m_FreeMixTracks.size() == 0)
		{
			for (MIX_Track* track : m_AllMixTracks)
			{
				if (!MIX_TrackPlaying(track))
				{
					m_FreeMixTracks.emplace_back(track);
				}
			}

			if (m_FreeMixTracks.size() == 0)
			{
				MIX_Track* newTrack = MIX_CreateTrack(m_Mixer);
				m_AllMixTracks.emplace_back(newTrack);
				m_FreeMixTracks.emplace_back(newTrack);
			}
		}
		MIX_Track* track = m_FreeMixTracks[m_FreeMixTracks.size() -1];
		//m_FreeMixTracks.erase(std::remove(m_FreeMixTracks.begin(), m_FreeMixTracks.end(), track), m_FreeMixTracks.end());
		m_FreeMixTracks.erase(m_FreeMixTracks.end() - 1, m_FreeMixTracks.end());
		return track;
	}
}