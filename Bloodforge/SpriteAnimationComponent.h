#pragma once
#include "Component.h"
#include "Rect.h"
#include <functional>

namespace Bloodforge
{
	class Texture2D;

	struct AnimationEventData
	{
		int FrameToTrigger = 0;
		float Offset = 0.0f;
		bool HasBeenTriggered = false;
		int CallbackIndex = 0;
	};

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
		std::vector<AnimationEventData> AnimationEvents;
	};

	namespace AnimationUtils
	{
		void InitializeAnimation(SpriteAnimationComponent& animation, Texture2D* texture, int numberOfFrames);
		
		void Play(SpriteAnimationComponent& animation);
		void Pause(SpriteAnimationComponent& animation);
		void Resume(SpriteAnimationComponent& animation);
		void Stop(SpriteAnimationComponent& animation);

		void AdvanceToNextFrame(SpriteAnimationComponent& animation);
		void RegressToPreviousFrame(SpriteAnimationComponent& animation);

		void AddAnimationEvent(SpriteAnimationComponent& animation, std::function<void()> callback, int indexOfFrameToTriggerOn, float offset = 0.0f);
	}
}