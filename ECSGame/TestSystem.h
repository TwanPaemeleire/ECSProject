#pragma once
#include <System.h>

class TestSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
};

