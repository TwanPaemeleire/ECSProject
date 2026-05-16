#pragma once
#include <System.h>

namespace Bloodforge
{
	struct InputActionInfo;
}

class InputTesterSystem final : public Bloodforge::System
{
public:
	virtual void OnStart() override;

private:
	void OnMouseAction(const Bloodforge::InputActionInfo& info);
};