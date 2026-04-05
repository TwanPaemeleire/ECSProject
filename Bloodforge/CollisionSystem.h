#pragma once
#include "System.h"
#include <vector>

namespace Bloodforge
{
	struct ColliderRect;
	struct Vector2;
	struct RectColliderComponent;
	struct TransformComponent;
	class CollisionSystem final : public System
	{
	public:
		virtual void OnLateUpdate() override;
		virtual void OnRender() override;

	private:
		void UpdateAllCollisionRects();
		bool IsOverlapping(const ColliderRect& rect1, const ColliderRect& rect2);

		void ProjectRectOntoAxis(const ColliderRect& rect, const Vector2& axis, float& min, float& max);
		bool IsOverlappingOnAxis(const ColliderRect& rect1, const ColliderRect& rect2, const Vector2& axis);
		std::vector<std::pair<int, int>> m_LastFrameCollisions;
	};
}