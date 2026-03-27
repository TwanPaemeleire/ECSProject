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
				Bloodforge::GlobalEventHandler::GetInstance().InvokeEvent<TestGlobalEventData>(data);
				m_TestEvent->Invoke(0.2f, 2);

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