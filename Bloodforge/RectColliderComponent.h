#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Event.h"
#include <memory>

namespace Bloodforge
{
	struct Rectf
	{
		float X;
		float Y;
		float Width;
		float Height;
		Vector2 TopLeft;
		Vector2 TopRight;
		Vector2 BottomRight;
		Vector2 BottomLeft;
	};

	struct RectColliderComponent final :Component<RectColliderComponent>
	{
		RectColliderComponent()
		{
			OnCollisionEnterEvent = std::make_unique<Event<int>>();
			OnCollisionEvent = std::make_unique<Event<int>>();
			OnCollisionExitEvent = std::make_unique<Event<int>>();
		}

		const Rectf& GetRect();

		void SetSize(const Vector2& size)
		{
			Size = size;
			WorldPosNeedsUpdate = true;
		}

		void SetOffset(const Vector2& offset)
		{
			Offset = offset;
			WorldPosNeedsUpdate = true;
		}

		void UpdateWorldSpaceRect();

		std::unique_ptr<Event<int>> OnCollisionEnterEvent;
		std::unique_ptr<Event<int>> OnCollisionEvent;
		std::unique_ptr<Event<int>> OnCollisionExitEvent;

	private:
		Vector2 RotateRectPoint(float x, float y, float cosRot, float sinRot);

		Vector2 Size;
		Vector2 Offset;

		Rectf WorldSpaceRect = {};
		bool WorldPosNeedsUpdate = false;
	};
}