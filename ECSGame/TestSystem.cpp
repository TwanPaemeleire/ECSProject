#include "TestSystem.h"
#include <EntityManager.h>
#include <TransformComponent.h>
#include <SpriteComponent.h>
#include "RotationComponent.h"
#include <BloodTime.h>
#include <GlobalEventHandler.h>
#include <iostream>
#include <numbers>
#include <WindowUtils.h>
#include <RectColliderComponent.h>

void TestSystem::OnUpdate()
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();

	for (Bloodforge::ChunkView view : result.Chunks)
	{
		for (int i = 0; i < view.GetComponentArray<Bloodforge::TransformComponent>().size(); ++i)
		{
			RotationComponent& rotationComp = view.GetComponentArray<RotationComponent>()[i];
			Bloodforge::TransformComponent& transformComp = view.GetComponentArray<Bloodforge::TransformComponent>()[i];
			if (rotationComp.Progress >= 1.0f)
			{
				rotationComp.Progress = 0.0f;
				std::swap(rotationComp.StartPosition, rotationComp.EndPosition);
			}

			rotationComp.Progress += Bloodforge::BloodTime::GetInstance().DeltaTime / rotationComp.TimeToReach;
			rotationComp.Progress = std::clamp(rotationComp.Progress, 0.0f, 1.0f);
			Bloodforge::Vector2 newPos;
			if(rotationComp.SmoothLerp) newPos = Bloodforge::Vector2::SmoothLerp(rotationComp.StartPosition, rotationComp.EndPosition, rotationComp.Progress);
			else newPos = Bloodforge::Vector2::Lerp(rotationComp.StartPosition, rotationComp.EndPosition, rotationComp.Progress);
			transformComp.SetLocalPosition(newPos);
		}
	}
}