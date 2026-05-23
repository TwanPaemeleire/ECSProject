#pragma once
#include "System.h"
#include "Vector2.h"

namespace Bloodforge
{
	struct InputActionInfo;
	struct ButtonComponent;
	struct TransformComponent;
	class ButtonSystem final : public System
	{
	public:
		ButtonSystem();
		virtual void OnUpdate() override;

	private:
		void OnLeftMouseButtonDown(const InputActionInfo& actionInfo);
		void OnMouseMove(const InputActionInfo& actionInfo);

		bool MouseIsOverButton(const ButtonComponent& buttonComp, TransformComponent& transformComp, Vector2 mousePos);
	};
}