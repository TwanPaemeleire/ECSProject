#include "pch.h"
#include "CameraSystem.h"
#include "EntityManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "BloodTime.h"

void Bloodforge::CameraSystem::OnLateUpdate()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityView<CameraComponent, TransformComponent> view = entityManager.GetOrCreateFirstEntityWithComponents<CameraComponent, TransformComponent>();
	TransformComponent& transformComp = view.GetComponent<TransformComponent>();
	Vector2 pos = transformComp.GetLocalPosition();
	pos.X += 20.0f * BloodTime::GetInstance().DeltaTime;
	transformComp.SetLocalPosition(pos);
}