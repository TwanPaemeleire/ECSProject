#include "TestSystem.h"
#include <EntityManager.h>
#include <TransformComponent.h>
#include "RotationComponent.h"
#include <BloodTime.h>
#include <RectColliderComponent.h>
#include <iostream>

void TestSystem::OnStart()
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::RectColliderComponent>();

	for (Bloodforge::ChunkView view : result.Chunks)
	{
		for (int i = 0; i < view.GetComponentArray<Bloodforge::RectColliderComponent>().size(); ++i)
		{
			Bloodforge::RectColliderComponent& colliderComp = view.GetComponentArray<Bloodforge::RectColliderComponent>()[i];
			colliderComp.OnCollisionEnterEvent.AddListener(std::bind(&TestSystem::OnCollisionStart, this));
			colliderComp.OnCollisionEvent.AddListener(std::bind(&TestSystem::OnCollision, this));
			colliderComp.OnCollisionExitEvent.AddListener(std::bind(&TestSystem::OnCollisionEnd, this));
		}
	}
}

void TestSystem::OnUpdate()
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::TransformComponent, RotationComponent, Bloodforge::RectColliderComponent>();

	for (Bloodforge::ChunkView view : result.Chunks)
	{
		for (int i = 0; i < view.GetComponentArray<Bloodforge::TransformComponent>().size(); ++i)
		{
			RotationComponent& rotationComp = view.GetComponentArray<RotationComponent>()[i];
			Bloodforge::TransformComponent& transformComp = view.GetComponentArray<Bloodforge::TransformComponent>()[i];
			transformComp.Rotate(rotationComp.SpeedDegPerSec * Bloodforge::BloodTime::GetInstance().DeltaTime);
		}
	}
}

void TestSystem::OnCollisionStart()
{
	std::cout << "Collision Started!" << std::endl;
}

void TestSystem::OnCollision()
{
	std::cout << "Colliding!" << std::endl;
}

void TestSystem::OnCollisionEnd()
{
	std::cout << "Collision Ended!" << std::endl;
}