#include "pch.h"
#include "RectColliderComponent.h"
#include "EntityManager.h"
#include "TransformComponent.h"

namespace Bloodforge
{
	const Rectf& RectColliderComponent::GetRect()
	{
		if (WorldPosNeedsUpdate)
		{
			UpdateWorldSpaceRect();
		}
		return WorldSpaceRect;
	}
	void RectColliderComponent::UpdateWorldSpaceRect()
	{
		EntityManager& entityManager = EntityManager::GetInstance();
		TransformComponent* transformComponent = entityManager.GetComponent<TransformComponent>(entityManager.GetEntity(OwnerEntityId));

		const Vector2& worldPos = transformComponent->GetWorldPosition();
		WorldSpaceRect.X = worldPos.X;
		WorldSpaceRect.Y = worldPos.Y;
		WorldSpaceRect.Width = Size.X;
		WorldSpaceRect.Height = Size.Y;

		WorldSpaceRect.X -= Size.X / 2.0f;
		WorldSpaceRect.Y -= Size.Y / 2.0f;

		WorldSpaceRect.X += Offset.X;
		WorldSpaceRect.Y += Offset.Y;

		WorldPosNeedsUpdate = false;
	}
}