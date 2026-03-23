#pragma once
#include <Component.h>
#include <Vector2.h>

struct RotationComponent final : public Bloodforge::Component<RotationComponent>
{
	float Angle = 0.0f;
	Bloodforge::Vector2 CenterPosition = {};
	float Speed = 20.0f;
	float Radius = 80.0f;
};