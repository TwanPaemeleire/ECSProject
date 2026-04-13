#include "pch.h"
#include "SpriteAnimatorComponent.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "SpriteAnimatorSystem.h"
#include <utility>

namespace Bloodforge::AnimationUtils
{
	void AnimationUtils::AddAnimation(SpriteAnimatorComponent& animation, AnimationId id, const AnimationData& animationData)
	{
		if (!animation.AnimationsData.contains(id))
		{
			animation.AnimationsData.insert({ id, animationData });
		}
		else
		{
			throw std::exception("Trying to add an animation with an id that has already been added.");
		}
	}

	void PlayAnimation(SpriteAnimatorComponent& animation, AnimationId id)
	{
		if (!animation.AnimationsData.contains(id))
		{
			throw std::exception("Trying to play an animation with an id that hasn't been added.");
		}

		AnimationData& data = animation.AnimationsData[id];
		for (AnimationEventData& eventData : data.AnimationEvents)
		{
			eventData.HasBeenTriggered = false;
		}

		animation.IsPlaying = true;
		animation.CurrentFrame = data.StartingFrame;
		animation.FrameTimeCounter = 0.0f;
		animation.CurrentPlayingId = id;
		animation.CurrentFrameWidth = data.Texture->GetWidth() / data.NumberOfFrames;
		animation.CurrentFrameHeight = data.Texture->GetHeight();
		animation.CurrentTexture = data.Texture;
		animation.CurrentSourceRect.X = data.StartingFrame * animation.CurrentFrameWidth;
		animation.CurrentSourceRect.Y = 0.0f;
		animation.CurrentSourceRect.Width = animation.CurrentFrameWidth;
		animation.CurrentSourceRect.Height = animation.CurrentFrameHeight;
	}

	void Pause(SpriteAnimatorComponent& animation)
	{
		if (!animation.IsPlaying) return;
		animation.IsPaused = true;
	}

	void Resume(SpriteAnimatorComponent& animation)
	{
		if (!animation.IsPlaying) return;
		animation.IsPaused = false;
	}

	void Stop(SpriteAnimatorComponent& animation)
	{
		animation.IsPlaying = false;
		animation.IsPaused = false;
	}

	void AdvanceToNextFrame(SpriteAnimatorComponent& animation)
	{
		const AnimationData& data = animation.AnimationsData[animation.CurrentPlayingId];
		if (animation.CurrentFrame < data.NumberOfFrames - 1) // Go to next frame, end of animation not reached yet
		{
			++animation.CurrentFrame;
			animation.FrameTimeCounter = 0.0f;
		}
		else // Go to next frame, end of animation has been reached
		{
			if (data.ShouldLoop)
			{
				animation.CurrentFrame = data.StartingFrameIndexAfterLoop;
				animation.FrameTimeCounter = 0.0f;

			}
			else
			{
				Pause(animation);
				animation.CurrentFrame = data.NumberOfFrames - 1;
				animation.FrameTimeCounter = 0.0f;
			}
		}

		animation.CurrentSourceRect.X = animation.CurrentFrame * animation.CurrentFrameWidth;
		animation.CurrentSourceRect.Y = 0.0f;
	}

	void RegressToPreviousFrame(SpriteAnimatorComponent& animation)
	{
		if (animation.CurrentFrame > 0)
		{
			--animation.CurrentFrame;
			animation.FrameTimeCounter = 0.0f;
		}
	}

	void AddAnimationEvent(SpriteAnimatorComponent& animation, AnimationId id, std::function<void(SpriteAnimatorComponent&)> callback, int frameToTrigger, float offset)
	{
		SpriteAnimatorSystem* animSystem = SceneManager::GetInstance().GetActiveScene().GetSystem<SpriteAnimatorSystem>();
		if (!animation.AnimationsData.contains(id))
		{
			throw std::exception("Trying to add animation event to an animation with an id that doesn't exist");
		}
		animation.AnimationsData[id].AnimationEvents.push_back({frameToTrigger, offset, false, animSystem->AddAnimationEvent(std::move(callback))});
	}
}