#pragma once
#include "Component.h"
#include "Color.h"

namespace Bloodforge
{
	class Texture2D;

	struct SpriteComponent final : public Component<SpriteComponent>
	{
		SpriteComponent() = default;
		~SpriteComponent() = default;

		Texture2D* Texture = nullptr;
		Color Color{};
	};
}