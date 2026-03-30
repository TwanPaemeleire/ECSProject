#pragma once
#include <Component.h>
#include <Vector2.h>

struct RotationComponent final : public Bloodforge::Component<RotationComponent>
{
	Bloodforge::Vector2 StartPosition = {};
	Bloodforge::Vector2 EndPosition = {};
	float TimeToReach = 20.0f;
	float Progress = 0.0f;
	bool SmoothLerp = true;
};