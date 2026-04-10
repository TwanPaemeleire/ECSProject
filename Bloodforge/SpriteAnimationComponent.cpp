#include "pch.h"
#include "SpriteAnimationComponent.h"
#include "Texture2D.h"
#include <iostream>

namespace Bloodforge::AnimationUils
{
	void InitializeAnimation(SpriteAnimationComponent& animation, Texture2D* texture, int numberOfFrames)
	{
		animation.Texture = texture;
		animation.NumberOfFrames = numberOfFrames;
		animation.FrameWidth = animation.Texture->GetWidth() / animation.NumberOfFrames;
		animation.FrameHeight = animation.Texture->GetHeight();
		animation.FrameTimeCounter = 0.0f;
		animation.CurrentFrame = animation.StartingFrame;
		animation.CurrentSourceRect.Width = animation.FrameWidth;
		animation.CurrentSourceRect.Height = animation.FrameHeight;
	}

	void Play(SpriteAnimationComponent& animation)
	{
		animation.IsPlaying = true;
		animation.CurrentFrame = animation.StartingFrame;
		animation.FrameTimeCounter = 0.0f;
	}

	void Pause(SpriteAnimationComponent& animation)
	{
		animation.IsPlaying = false;
	}

	void Resume(SpriteAnimationComponent& animation)
	{
		animation.IsPlaying = true;
	}

	void Stop(SpriteAnimationComponent& animation)
	{
		animation.IsPlaying = false;
		animation.CurrentFrame = animation.StartingFrame;
		animation.FrameTimeCounter = 0.0f;
	}

	void AdvanceToNextFrame(SpriteAnimationComponent& animation)
	{
		if (animation.CurrentFrame < animation.NumberOfFrames - 1) // Go to next frame, end of animation not reached yet
		{
			++animation.CurrentFrame;
			animation.FrameTimeCounter = 0.0f;
		}
		else // Go to next frame, end of animation has been reached
		{
			if (animation.ShouldLoop)
			{
				animation.CurrentFrame = animation.StartingFrameIndexAfterLoop;
				animation.FrameTimeCounter = 0.0f;

			}
			else
			{
				Pause(animation);
				animation.CurrentFrame = animation.NumberOfFrames - 1;
				animation.FrameTimeCounter = 0.0f;
			}
		}

		animation.CurrentSourceRect.X = animation.CurrentFrame * animation.FrameWidth;
		animation.CurrentSourceRect.Y = 0.0f;
	}

	void RegressToPreviousFrame(SpriteAnimationComponent & animation)
	{
		if (animation.CurrentFrame > 0)
		{
			--animation.CurrentFrame;
			animation.FrameTimeCounter = 0.0f;
		}
	}
}