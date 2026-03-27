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

void TestSystem::OnStart()
{
	// Global event
	auto& globalEventHandler = Bloodforge::GlobalEventHandler::GetInstance();
	int globalId = globalEventHandler.AddListener<TestGlobalEventData>([this](const TestGlobalEventData& eventData)
		{
			GlobalEventTest(eventData);
		});

	globalEventHandler.RemoveListener<TestGlobalEventData>(globalId);
	globalEventHandler.AddListener<TestGlobalEventData>([this](const TestGlobalEventData& eventData)
		{
			GlobalEventTest(eventData);
		});

	// Local event
	m_TestEvent = std::make_unique<Bloodforge::Event<float, int>>();
	int id = m_TestEvent->AddListener([this](float x, int y)
		{
			LocalEventTest(x, y);
		});
	m_TestEvent->RemoveListener(id);
	m_TestEvent->AddListener([this](float x, int y)
		{
			LocalEventTest(x, y);
		});


	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::EntityQueryResult<Bloodforge::RectColliderComponent> result = entityManager.QueryEntities<Bloodforge::RectColliderComponent>();

	for (Bloodforge::ChunkView<Bloodforge::RectColliderComponent> view : result.Chunks)
	{
		for (int i = 0; i < view.GetComponentArray<Bloodforge::RectColliderComponent>().size(); ++i)
		{
			Bloodforge::RectColliderComponent& rectCollider = view.GetComponentArray<Bloodforge::RectColliderComponent>()[i];
			rectCollider.OnCollisionEnterEvent->AddListener([this](int entityId)
				{
					OnCollisionEnterEvent(entityId);
				});
			rectCollider.OnCollisionEvent->AddListener([this](int entityId)
				{
					OnCollisionEvent(entityId);
				});
			rectCollider.OnCollisionExitEvent->AddListener([this](int entityId)
				{
					OnCollisionExitEvent(entityId);
				});
		}
	}

}

void TestSystem::OnUpdate()
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();

	for (Bloodforge::ChunkView view : result.Chunks)
	{
		for (int i = 0; i < view.GetComponentArray<Bloodforge::TransformComponent>().size(); ++i)
		{
			RotationComponent& rotationComp = view.GetComponentArray<RotationComponent>()[i];
			rotationComp.Angle += rotationComp.Speed * Bloodforge::BloodTime::GetInstance().DeltaTime;
			float deltaX = rotationComp.Radius * std::cos(rotationComp.Angle);
			float deltaY = rotationComp.Radius * std::sin(rotationComp.Angle);
			float x = rotationComp.CenterPosition.X + deltaX;
			float y = rotationComp.CenterPosition.Y + deltaY;
			view.GetComponentArray<Bloodforge::TransformComponent>()[i].SetLocalPosition(x, y);

			if (rotationComp.Angle >= std::numbers::pi * 2)
			{
				rotationComp.Angle -= static_cast<float>(std::numbers::pi) * 2.0f;
				TestGlobalEventData data;
				data.Test = 20;
				// Bloodforge::GlobalEventHandler::GetInstance().InvokeEvent<TestGlobalEventData>(data);
				// m_TestEvent->Invoke(0.2f, 2);

				if (m_TestFlag)
				{
					// Bloodforge::WindowUtils::SetWindowSize(400, 400);
				}
				else 
				{
					// Bloodforge::WindowUtils::SetWindowSize(800, 800);
				}
				m_TestFlag = !m_TestFlag;
			}
		}
	}
}

void TestSystem::GlobalEventTest(const TestGlobalEventData&)
{
	std::cout << "Global event triggered" << std::endl;
}

void TestSystem::LocalEventTest(float, int)
{
	std::cout << "Local event triggered" << std::endl;
}

void TestSystem::OnCollisionEnterEvent(int otherEntityId)
{
	std::cout << "Collision enter event with entity id: " << otherEntityId << std::endl;
}

void TestSystem::OnCollisionEvent(int otherEntityId)
{
	std::cout << "Collision event with entity id: " << otherEntityId << std::endl;
}

void TestSystem::OnCollisionExitEvent(int otherEntityId)
{
	std::cout << "Collision exit event with entity id: " << otherEntityId << std::endl;
}
