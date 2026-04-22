#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Event.h"
#include <memory>

using TagId = unsigned int;

namespace Bloodforge
{
	struct ColliderRect
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
		const ColliderRect& GetRect();

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

		Event<int> OnCollisionEnterEvent;
		Event<int> OnCollisionEvent;
		Event<int> OnCollisionExitEvent;

		std::vector<TagId> IgnoreTags;

	private:
		Vector2 RotateRectPoint(float x, float y, float cosRot, float sinRot);

		Vector2 Size;
		Vector2 Offset;

		ColliderRect WorldSpaceRect = {};
		bool WorldPosNeedsUpdate = false;
	};
}