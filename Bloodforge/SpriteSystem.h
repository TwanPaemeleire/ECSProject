#pragma once
#include "System.h"

namespace Bloodforge 
{
	struct InputActionInfo;

	class SpriteSystem final : public System
	{
	public:
		virtual void OnRender() override;
	};
}