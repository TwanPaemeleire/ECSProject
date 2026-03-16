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
	};
}