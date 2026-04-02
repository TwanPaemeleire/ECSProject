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
			// RotationComponent& rotationComp = view.GetComponentArray<RotationComponent>()[i];
			Bloodforge::TransformComponent& transformComp = view.GetComponentArray<Bloodforge::TransformComponent>()[i];
			transformComp.Rotate(90.0f * Bloodforge::BloodTime::GetInstance().DeltaTime);
			transformComp.SetLocalScale(transformComp.GetLocalScale() * 0.9999f);
		}
	}
}