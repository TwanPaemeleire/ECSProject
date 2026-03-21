#pragma once
#include "System.h"

namespace Bloodforge 
{
	struct InputActionInfo;

	class SpriteSystem final : public System
	{
	public:
		virtual void OnStart() override;
		virtual void OnRender() override;

		void InputTest(const InputActionInfo& info);
	private:
		float m_Speed = 2;
	};
}