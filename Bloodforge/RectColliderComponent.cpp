#include "pch.h"
#include "RectColliderComponent.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <stdexcept>
#include "Texture2D.h"

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
		SpriteComponent* spriteComponent = entityManager.GetComponent<SpriteComponent>(entityManager.GetEntity(OwnerEntityId));

		if (CenterToSprite && spriteComponent == nullptr)
		{
			throw std::runtime_error("Trying to center a rect collider to a sprite on an entity that does not have a SpriteComponent");
			return;
		}

		const Vector2& worldPos = transformComponent->GetWorldPosition();
		WorldSpaceRect.X = worldPos.X;
		WorldSpaceRect.Y = worldPos.Y;
		WorldSpaceRect.Width = Size.X;
		WorldSpaceRect.Height = Size.Y;

		if (CenterToSprite)
		{
			const Vector2& spriteSize = spriteComponent->Texture->GetSize();
			WorldSpaceRect.X += spriteSize.X / 2.0f - Size.X / 2.0f;
			WorldSpaceRect.Y += spriteSize.Y / 2.0f - Size.Y / 2.0f;
		}

		WorldSpaceRect.X += Offset.X;
		WorldSpaceRect.Y += Offset.Y;

		WorldPosNeedsUpdate = false;
	}
}