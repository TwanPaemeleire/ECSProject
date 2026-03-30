#pragma once
#include <System.h>
#include <Event.h>
#include <memory>

class TestSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
private:
};