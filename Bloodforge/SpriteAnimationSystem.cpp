#include "pch.h"
#include "SpriteAnimationSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "SpriteAnimationComponent.h"
#include "BloodTime.h"
#include <utility>

namespace Bloodforge
{
	void SpriteAnimationSystem::OnStart()
	{
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<SpriteAnimationComponent, SpriteComponent>();
		for (ChunkView view : result.Chunks)
		{
			for (int i = 0; i < view.GetComponentArray<SpriteAnimationComponent>().size(); ++i)
			{
				SpriteComponent& spriteComp = view.GetComponentArray<SpriteComponent>()[i];
				SpriteAnimationComponent& animationComp = view.GetComponentArray<SpriteAnimationComponent>()[i];
				if (animationComp.PlayOnStart)
				{
					AnimationUtils::Play(animationComp);
				}

				spriteComp.SetTexture(animationComp.Texture);
			}
		}
	}

	void SpriteAnimationSystem::OnUpdate()
	{
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<SpriteAnimationComponent, SpriteComponent>();
		for (ChunkView view : result.Chunks)
		{
			for (int i = 0; i < view.GetComponentArray<SpriteAnimationComponent>().size(); ++i)
			{
				SpriteComponent& spriteComp = view.GetComponentArray<SpriteComponent>()[i];
				SpriteAnimationComponent& animationComp = view.GetComponentArray<SpriteAnimationComponent>()[i];
				if (animationComp.IsPlaying)
				{
					animationComp.FrameTimeCounter += BloodTime::GetInstance().DeltaTime;
					HandleAnimationEvents(animationComp);
					if (animationComp.FrameTimeCounter >= animationComp.FrameTime)
					{
						AnimationUtils::AdvanceToNextFrame(animationComp);

						if (animationComp.CurrentFrame == 0)
						{
							ResetAllAnimationEvents(animationComp);
						}
					}

					spriteComp.SetCustomSourceRect(animationComp.CurrentSourceRect);
					spriteComp.SetTexture(animationComp.Texture);
				}
			}
		}
	}

	int SpriteAnimationSystem::AddAnimationEvent(std::function<void()> callback)
	{
		m_AnimationEventsCallbacks.insert({ m_NextAnimationEventCallbackID, std::move(callback) });
		return m_NextAnimationEventCallbackID++;
	}

	void SpriteAnimationSystem::HandleAnimationEvents(SpriteAnimationComponent& spriteAnimComp)
	{
		for (AnimationEventData& animEventData : spriteAnimComp.AnimationEvents)
		{
			if (!animEventData.HasBeenTriggered && spriteAnimComp.CurrentFrame == animEventData.FrameToTrigger && spriteAnimComp.FrameTimeCounter >= animEventData.Offset)
			{
				m_AnimationEventsCallbacks[animEventData.CallbackIndex]();
				animEventData.HasBeenTriggered = true;
			}
		}
	}
	void SpriteAnimationSystem::ResetAllAnimationEvents(SpriteAnimationComponent& spriteAnimComp)
	{
		for (AnimationEventData& animEventData : spriteAnimComp.AnimationEvents)
		{
			animEventData.HasBeenTriggered = false;
		}
	}
}