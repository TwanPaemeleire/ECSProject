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
		std::unordered_set<uint64_t> currentFrameCollisions;
		EntityManager& entityManager = EntityManager::GetInstance();
		EntityQueryResult result = entityManager.QueryEntities<RectColliderComponent, TransformComponent>();
		for (size_t chunkIndex = 0; chunkIndex < result.Chunks.size(); ++chunkIndex)
		{
			auto& view = result.Chunks[chunkIndex];
			std::span<RectColliderComponent> rectArray1 = view.GetComponentArray<RectColliderComponent>();
			std::span<TransformComponent> transformArray1 = view.GetComponentArray<TransformComponent>();

			for (size_t innerChunkIndex = chunkIndex; innerChunkIndex < result.Chunks.size(); ++innerChunkIndex)
			{
				auto& innerView = result.Chunks[innerChunkIndex];
				std::span<RectColliderComponent> rectArray2 = innerView.GetComponentArray<RectColliderComponent>();
				std::span<TransformComponent> transformArray2 = innerView.GetComponentArray<TransformComponent>();
				for (int index = 0; index < rectArray1.size(); ++index)
				{
					int innerIndexStart = (chunkIndex == innerChunkIndex) ? index + 1 : 0;
					RectColliderComponent& rect1 = rectArray1[index];
					TransformComponent& transform1 = transformArray1[index];
					for (int innerIndex = innerIndexStart; innerIndex < rectArray2.size(); ++innerIndex)
					{
						RectColliderComponent& rect2 = rectArray2[innerIndex];
						TransformComponent& transform2 = transformArray2[innerIndex];
						if (rect1.OwnerEntityId == rect2.OwnerEntityId) continue; // Checking collider against itself
						
						Entity& entity1 = entityManager.GetEntity(rect1.OwnerEntityId);
						Entity& entity2 = entityManager.GetEntity(rect2.OwnerEntityId);
						bool rect1IgnoresRect2 = std::find(rect1.IgnoreTags.begin(), rect1.IgnoreTags.end(), entity2.Tag) != rect1.IgnoreTags.end();
						bool rect2IgnoresRect1 = std::find(rect2.IgnoreTags.begin(), rect2.IgnoreTags.end(), entity1.Tag) != rect2.IgnoreTags.end();
						if (rect1IgnoresRect2 && rect2IgnoresRect1) continue; // If they both ignore each other, no need to check collision

						if (IsOverlappingAABB(rect1.GetRect(), rect2.GetRect())) 
						{
							if ((transform1.GetWorldRotation() == 0.0f && transform2.GetWorldRotation() == 0.0f) || IsOverlapping(rect1.GetRect(), rect2.GetRect()))
							{
								std::pair<int, int> pair = { transform1.OwnerEntityId, transform2.OwnerEntityId };
								currentFrameCollisions.insert(Pack(transform1.OwnerEntityId, transform2.OwnerEntityId));
								// Were not colliding last frame
								if (!m_LastFrameCollisions.contains(Pack(transform1.OwnerEntityId, transform2.OwnerEntityId)))
								{
									if (!rect1IgnoresRect2) rect1.OnCollisionEnterEvent.Invoke(rect2.OwnerEntityId);
									if (!rect2IgnoresRect1) rect2.OnCollisionEnterEvent.Invoke(rect1.OwnerEntityId);
								}

								// Collision event gets called every frame
								if (!rect1IgnoresRect2) rect1.OnCollisionEvent.Invoke(rect2.OwnerEntityId);
								if (!rect2IgnoresRect1) rect2.OnCollisionEvent.Invoke(rect1.OwnerEntityId);

							}
						}
					}
				}
			}
		}

		// Check for collision exits
		for (const auto& lastFramePair : m_LastFrameCollisions)
		{
			if(!currentFrameCollisions.contains(lastFramePair))
			{
				int first = UnpackFirst(lastFramePair);
				int second = UnpackSecond(lastFramePair);
				Entity& firstEntity = entityManager.GetEntity(first);
				Entity& secondEntity = entityManager.GetEntity(second);
				RectColliderComponent* firstRect = entityManager.GetComponent<RectColliderComponent>(firstEntity);
				RectColliderComponent* secondRect = entityManager.GetComponent<RectColliderComponent>(secondEntity);
				bool firstIgnoresSecond = std::find(firstRect->IgnoreTags.begin(), firstRect->IgnoreTags.end(), secondEntity.Tag) != firstRect->IgnoreTags.end();
				bool secondIgnoresFirst = std::find(secondRect->IgnoreTags.begin(), secondRect->IgnoreTags.end(), firstEntity.Tag) != secondRect->IgnoreTags.end();
				if(!firstIgnoresSecond) firstRect->OnCollisionExitEvent.Invoke(second);
				if(!secondIgnoresFirst) secondRect->OnCollisionExitEvent.Invoke(first);
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

	bool CollisionSystem::IsOverlappingAABB(const ColliderRect& rect1, const ColliderRect& rect2)
	{
		if (rect1.TopRight.X < rect2.TopLeft.X || rect1.TopLeft.X > rect2.TopRight.X) return false;
		if (rect1.TopRight.Y < rect2.TopLeft.Y || rect1.TopLeft.Y > rect2.TopRight.Y) return false;
		return true;
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

	uint64_t CollisionSystem::Pack(int a, int b)
	{
		return (static_cast<uint64_t>(a) << 32) | b;
	}

	int CollisionSystem::UnpackFirst(uint64_t packed)
	{
		return static_cast<int>(packed >> 32);
	}

	int CollisionSystem::UnpackSecond(uint64_t packed)
	{
		return static_cast<int>(packed & 0xFFFFFFFF);
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