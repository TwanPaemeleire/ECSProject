#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <vector>

using SoundId = unsigned int;

struct MIX_Track;
struct MIX_Audio;
struct MIX_Mixer;

namespace Bloodforge
{
	class SoundPlayer final : public Singleton<SoundPlayer>
	{
	public:
		~SoundPlayer();

		void RequestLoadSound(const std::string& path, SoundId id);
		void RequestPlaySound(SoundId id, const float volume);
		void RequestStopAllSound(int fadeOutTimeInMs);
	private:
		friend class Singleton<SoundPlayer>;
		explicit SoundPlayer();

		MIX_Track* GetAvailableMixTrack();

		MIX_Mixer* m_Mixer;
		std::unordered_map<SoundId, MIX_Audio*> m_LoadedAudio;

		std::vector<MIX_Track*> m_AllMixTracks;
		std::vector<MIX_Track*> m_FreeMixTracks;
	};
}