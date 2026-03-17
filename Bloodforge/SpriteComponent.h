#pragma once
#include "Component.h"

namespace Bloodforge
{
	class Texture2D;

	struct SpriteComponent final : public Component<SpriteComponent>
	{
		SpriteComponent() = default;
		~SpriteComponent() = default;

		Texture2D* Texture;
	};
}