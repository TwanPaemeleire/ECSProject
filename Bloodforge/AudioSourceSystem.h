#pragma once
#include "System.h"
#include <vector>

struct MIX_Mixer;
struct MIX_Track;
namespace Bloodforge
{
	struct AudioSource;
	class AudioSourceSystem final : public System
	{
	public:
		virtual void OnStart() override;

		void SetAudioOfTrack(AudioSource& audioSource);
		void StartPlayingAudioSource(AudioSource& audioSource);
		void StopPlayingAudioSource(AudioSource& audioSource);
		void PauseAudioSource(AudioSource& audioSource);
		void ResumeAudioSource(AudioSource& audioSource);

		void AddAudioSource(AudioSource& audioSource);
		void RemoveAudioSource(AudioSource& audioSource);
	private:
		int GetAvailableMixTrackIndex();

		MIX_Mixer* m_Mixer;
		std::vector<MIX_Track*> m_AllMixTracks;
		std::vector<int> m_FreeIndices;
	};
}