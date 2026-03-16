#pragma once
#include "Component.h"

namespace Bloodforge
{
	struct SpriteComponent : public Component<SpriteComponent>
	{
	public:
		SpriteComponent() = default;
		~SpriteComponent() = default;
	};
}