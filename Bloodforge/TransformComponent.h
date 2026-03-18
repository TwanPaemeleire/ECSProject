#pragma once
#include "Component.h"

namespace Bloodforge
{
	struct TransformComponent : Component<TransformComponent>
	{
	public:
		TransformComponent() = default;
		~TransformComponent() = default;
		int TestVar = 0;

		float X = 0.0f;
		float Y = 0.0f;
	};
}