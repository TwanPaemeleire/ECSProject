#include "TestSystem.h"
#include <EntityManager.h>
#include <TransformComponent.h>
#include <SpriteComponent.h>
#include "RotationComponent.h"
#include <BloodTime.h>

void TestSystem::OnUpdate()
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();

	for (Bloodforge::ChunkView view : result.Chunks)
	{
		for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
		{
			RotationComponent& rotationComp = std::get<2>(view.ComponentArrays)[i];
			rotationComp.Angle += rotationComp.Speed * Bloodforge::BloodTime::GetInstance().DeltaTime;
			float deltaX = rotationComp.Radius * std::cos(rotationComp.Angle);
			float deltaY = rotationComp.Radius * std::sin(rotationComp.Angle);
			float x = rotationComp.CenterPosition.X + deltaX;
			float y = rotationComp.CenterPosition.Y + deltaY;
			std::get<0>(view.ComponentArrays)[i].SetLocalPosition(x, y);
		}
	}
}
