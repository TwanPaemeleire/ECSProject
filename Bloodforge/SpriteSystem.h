#pragma once
#include "System.h"

namespace Bloodforge 
{
	class SpriteSystem final : public System
	{
	public:
		virtual void OnStart() override;
		virtual void OnRender() override;
	private:
		float m_Speed = 2;
	};
}