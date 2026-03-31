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
						TransformComponent& transform2 = transformArray1[innerIndex];
						if (rect1.OwnerEntityId == rect2.OwnerEntityId) continue; // Checking collider against itself

						if (IsOverlapping(transform1, rect1, transform2, rect2))
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
				const Rectf& collisionRect = rect.GetRect();
				BloodRenderer::GetInstance().DrawRectangle({ collisionRect.X, collisionRect.Y }, collisionRect.Width, collisionRect.Height, Color(255, 0, 0, 255));
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

	bool CollisionSystem::IsOverlapping(TransformComponent&, RectColliderComponent& rect1, TransformComponent&, RectColliderComponent& rect2)
	{
		const Rectf& rectHitBox1 = rect1.GetRect();
		const Rectf& rectHitBox2 = rect2.GetRect();

		return (rectHitBox1.X < rectHitBox2.X + rectHitBox2.Width)  &&
			   (rectHitBox1.X + rectHitBox1.Width > rectHitBox2.X)  && 
			   (rectHitBox1.Y < rectHitBox2.Y + rectHitBox2.Height) &&
			   (rectHitBox1.Y + rectHitBox1.Height > rectHitBox2.Y);
	}
}