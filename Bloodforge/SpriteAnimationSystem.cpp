#include "pch.h"
#include "SpriteAnimationSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "SpriteAnimationComponent.h"
#include "BloodTime.h"

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
					AnimationUils::Play(animationComp);
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
					if (animationComp.FrameTimeCounter >= animationComp.FrameTime)
					{
						AnimationUils::AdvanceToNextFrame(animationComp);
					}

					spriteComp.SetCustomSourceRect(animationComp.CurrentSourceRect);
					spriteComp.SetTexture(animationComp.Texture);
				}
			}
		}
	}
}