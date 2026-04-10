#pragma once
#include "System.h"

namespace Bloodforge
{
	class SpriteAnimationSystem final : public System
	{
	public:
		virtual void OnStart() override;
		virtual void OnUpdate() override;
	};
}