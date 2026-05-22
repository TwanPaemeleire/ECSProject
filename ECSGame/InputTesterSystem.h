#pragma once
#include <System.h>

namespace Bloodforge
{
	struct InputActionInfo;
}

class InputTesterSystem final : public Bloodforge::System
{
public:
	InputTesterSystem();

private:
	void OnLeftMouseAction(const Bloodforge::InputActionInfo& info);
	void OnMouseMotionAction(const Bloodforge::InputActionInfo& info);
};