#pragma once
#include <Component.h>
#include <Vector2.h>

struct Enemy final : public Bloodforge::Component<Enemy>
{
	Bloodforge::Vector2 Direction;
	float Speed = 0.0f;
};