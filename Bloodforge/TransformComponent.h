#pragma once
#include "Component.h"
#include "Vector2.h"
#include <vector>

namespace Bloodforge
{
	struct TransformComponent : Component<TransformComponent>
	{
	public:
		void SetLocalPosition(const Vector2& localPos);
		void SetLocalPosition(float x, float y);
		const Vector2& GetWorldPosition();
		const Vector2& GetLocalPosition() { return m_LocalPosition; }
		void SetPositionDirty();

		void SetParent(int parentEntityId, bool keepWorldPosition = true);

	private:
		void UpdateWorldPosition();

		void AddChild(int childEntityId);
		void RemoveChild(int childEntityId);
		bool IsChild(int childEntityId);

		int m_ParentEntityId = -1;
		std::vector<int> m_ChildEntityIds;

		bool m_PositionIsDirty{ false };
		Vector2 m_LocalPosition;
		Vector2 m_WorldPosition;
	};
}