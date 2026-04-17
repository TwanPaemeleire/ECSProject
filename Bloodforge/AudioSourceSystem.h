#pragma once
#include "System.h"
#include <vector>

struct MIX_Mixer;
struct MIX_Track;
namespace Bloodforge
{
	struct AudioSourceComponent;
	class AudioSourceSystem final : public System
	{
	public:
		virtual void OnStart() override;

		void SetAudioOfTrack(AudioSourceComponent& audioSource);
		void StartPlayingAudioSource(AudioSourceComponent& audioSource);
		void StopPlayingAudioSource(AudioSourceComponent& audioSource);
		void PauseAudioSource(AudioSourceComponent& audioSource);
		void ResumeAudioSource(AudioSourceComponent& audioSource);

		void AddAudioSource(AudioSourceComponent& audioSource);
		void RemoveAudioSource(AudioSourceComponent& audioSource);
	private:
		int GetAvailableMixTrackIndex();

		MIX_Mixer* m_Mixer;
		std::vector<MIX_Track*> m_AllMixTracks;
		std::vector<int> m_FreeIndices;
	};
}