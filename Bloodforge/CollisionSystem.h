#pragma once
#include "System.h"
#include <vector>

namespace Bloodforge
{
	struct Rectf;
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
		bool IsOverlapping(const Rectf& rect1, const Rectf& rect2);

		void ProjectRectOntoAxis(const Rectf& rect, const Vector2& axis, float& min, float& max);
		bool IsOverlappingOnAxis(const Rectf& rect1, const Rectf& rect2, const Vector2& axis);
		std::vector<std::pair<int, int>> m_LastFrameCollisions;
	};
}