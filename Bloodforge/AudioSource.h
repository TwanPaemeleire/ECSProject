#pragma once
#include "Component.h"

using SoundId = unsigned int;

namespace Bloodforge
{
	struct AudioSource final : Component<AudioSource>
	{
		AudioSource();
		void Play();
		void Pause();
		void Stop();
		void SetAudio(SoundId id);

		SoundId CurrentSoundId = 0;
		bool IsPlaying = false;
		bool IsPaused = false;
		bool PlayOnStart = false;
		bool ShouldLoop = false;
		float Volume = 1.0f;
		int TrackIndex = -1;
	};
}