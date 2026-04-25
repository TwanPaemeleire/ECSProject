#pragma once
#include <Component.h>

struct PlayerTower final : public Bloodforge::Component<PlayerTower>
{
	float Range = 200.0f;
	float Damage = 10.0f;
	float FireRateDelay = 0.02f;
	float TimeUntilNextShot = 0.0f;
};