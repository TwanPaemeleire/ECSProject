#include "pch.h"
#include "CollisionSystem.h"
#include "EntityManager.h"
#include "RectColliderComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"

namespace Bloodforge
{
	void CollisionSystem::OnLateUpdate()
	{
		UpdateAllCollisionRects();
		std::vector<std::pair<int, int>> currentFrameCollisions;
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<RectColliderComponent, TransformComponent>();
		for (ChunkView<RectColliderComponent, TransformComponent>& view : result.Chunks)
		{
			std::span<RectColliderComponent> rectArray1 = view.GetComponentArray<RectColliderComponent>();
			std::span<TransformComponent> transformArray1 = view.GetComponentArray<TransformComponent>();

			for (ChunkView<RectColliderComponent, TransformComponent>& innerView : result.Chunks)
			{
				std::span<RectColliderComponent> rectArray2 = innerView.GetComponentArray<RectColliderComponent>();
				std::span<TransformComponent> transformArray2 = innerView.GetComponentArray<TransformComponent>();

				for (int index = 0; index < rectArray1.size(); ++index)
				{
					RectColliderComponent& rect1 = rectArray1[index];
					TransformComponent& transform1 = transformArray1[index];
					for (int innerIndex = index + 1; innerIndex < rectArray2.size(); ++innerIndex)
					{
						RectColliderComponent& rect2 = rectArray2[innerIndex];
						TransformComponent& transform2 = transformArray2[innerIndex];
						if (rect1.OwnerEntityId == rect2.OwnerEntityId) continue; // Checking collider against itself

						if (IsOverlapping(rect1.GetRect(), rect2.GetRect()))
						{
							std::pair<int, int> pair = { transform1.OwnerEntityId, transform2.OwnerEntityId };
							currentFrameCollisions.emplace_back(pair);
							if (std::find(m_LastFrameCollisions.begin(), m_LastFrameCollisions.end(), pair) == m_LastFrameCollisions.end()) // Were not colliding last frame
							{
								rect1.OnCollisionEnterEvent->Invoke(rect2.OwnerEntityId);
								rect2.OnCollisionEnterEvent->Invoke(rect1.OwnerEntityId);
							}

							// Collision event gets called every frame
							rect1.OnCollisionEvent->Invoke(rect2.OwnerEntityId);
							rect2.OnCollisionEvent->Invoke(rect1.OwnerEntityId);

						}
					}
				}
			}
		}

		// Check for collision exits
		for (const auto& lastFramePair : m_LastFrameCollisions)
		{
			if (std::find(currentFrameCollisions.begin(), currentFrameCollisions.end(), lastFramePair) == currentFrameCollisions.end())
			{
				EntityManager& entityManager = EntityManager::GetInstance();
				int first = lastFramePair.first;
				int second = lastFramePair.second;
				entityManager.GetComponent<RectColliderComponent>(entityManager.GetEntity(first))->OnCollisionExitEvent->Invoke(second);
				entityManager.GetComponent<RectColliderComponent>(entityManager.GetEntity(second))->OnCollisionExitEvent->Invoke(first);
			}
		}

		m_LastFrameCollisions = std::move(currentFrameCollisions);
	}

	void CollisionSystem::OnRender()
	{
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<RectColliderComponent, TransformComponent>();
		for (ChunkView<RectColliderComponent, TransformComponent>& view : result.Chunks)
		{
			std::span<RectColliderComponent> rectArray = view.GetComponentArray<RectColliderComponent>();
			std::span<TransformComponent> transformArray = view.GetComponentArray<TransformComponent>();
			for (int index = 0; index < rectArray.size(); ++index)
			{
				// Draw debug rendering
				RectColliderComponent& rect = rectArray[index];
				const ColliderRect& collisionRect = rect.GetRect();
				BloodRenderer::GetInstance().DrawRectangle(collisionRect, Color(255, 255, 255, 255));
			}
		}
	}

	void CollisionSystem::UpdateAllCollisionRects()
	{
		EntityQueryResult<RectColliderComponent> result = EntityManager::GetInstance().QueryEntities<RectColliderComponent>();
		for (ChunkView<RectColliderComponent>& view : result.Chunks)
		{
			std::span<RectColliderComponent> rectArray = view.GetComponentArray<RectColliderComponent>();
			for (int index = 0; index < rectArray.size(); ++index)
			{
				rectArray[index].UpdateWorldSpaceRect();
			}
		}
	}

	bool CollisionSystem::IsOverlapping(const ColliderRect& rect1, const ColliderRect& rect2)
	{
		Vector2 axes[4]
		{
			(rect1.TopRight - rect1.TopLeft).GetPerpendicular().Normalized(),
			(rect1.BottomLeft - rect1.TopLeft).GetPerpendicular().Normalized(),
			(rect2.TopRight - rect2.TopLeft).GetPerpendicular().Normalized(),
			(rect2.BottomLeft - rect2.TopLeft).GetPerpendicular().Normalized()
		};

		for (const Vector2& axis : axes)
		{
			if (!IsOverlappingOnAxis(rect1, rect2, axis)) return false;
		}

		return true;
	}

	void CollisionSystem::ProjectRectOntoAxis(const ColliderRect& rect, const Vector2& axis, float& min, float& max)
	{
		const float points[4] = 
		{ 
			Vector2::Dot(rect.TopLeft, axis), 
			Vector2::Dot(rect.TopRight, axis),
			Vector2::Dot(rect.BottomRight, axis),
			Vector2::Dot(rect.BottomLeft, axis)
		};

		min = std::min(std::min(points[0], points[1]), std::min(points[2], points[3]));
		max = std::max(std::max(points[0], points[1]), std::max(points[2], points[3]));
	}

	bool CollisionSystem::IsOverlappingOnAxis(const ColliderRect& rect1, const ColliderRect& rect2, const Vector2& axis)
	{
		float minA;
		float maxA;
		float minB;
		float maxB;
		ProjectRectOntoAxis(rect1, axis, minA, maxA);
		ProjectRectOntoAxis(rect2, axis, minB, maxB);

		return !(maxA < minB || maxB < minA);
	}
}