#pragma once
#include "System.h"
#include <vector>

namespace Bloodforge
{
	struct RectColliderComponent;
	struct TransformComponent;
	class CollisionSystem final : public System
	{
	public:
		virtual void OnLateUpdate() override;
		virtual void OnRender() override;

	private:
		void UpdateAllCollisionRects();
		bool IsOverlapping(TransformComponent& transform1, RectColliderComponent& rect1, TransformComponent& transform2, RectColliderComponent& rect2);
		std::vector<std::pair<int, int>> m_LastFrameCollisions;
	};
}