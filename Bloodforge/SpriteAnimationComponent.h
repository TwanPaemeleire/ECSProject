#pragma once
#include "Component.h"
#include "Rect.h"

namespace Bloodforge
{
	class Texture2D;

	struct SpriteAnimationComponent final : public Component<SpriteAnimationComponent>
	{
		float FrameTime = 0.2f;
		int NumberOfFrames = 0;
		bool ShouldLoop = true;
		int StartingFrameIndexAfterLoop = 0;
		int StartingFrame = 0;
		bool PlayOnStart = true;

		Texture2D* Texture;
		Rect CurrentSourceRect;
		bool IsPlaying = false;
		float FrameTimeCounter = 0.0f;
		int CurrentFrame = 0;
		float FrameWidth = 0.0f;
		float FrameHeight = 0.0f;
	};

	namespace AnimationUils
	{
		void InitializeAnimation(SpriteAnimationComponent& animation, Texture2D* texture, int numberOfFrames);
		
		void Play(SpriteAnimationComponent& animation);
		void Pause(SpriteAnimationComponent& animation);
		void Resume(SpriteAnimationComponent& animation);
		void Stop(SpriteAnimationComponent& animation);

		void AdvanceToNextFrame(SpriteAnimationComponent& animation);
		void RegressToPreviousFrame(SpriteAnimationComponent& animation);
	}
}