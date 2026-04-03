#pragma once
#include <Component.h>

struct RotationComponent final : public Bloodforge::Component<RotationComponent>
{
	float SpeedDegPerSec = 90.f;
};