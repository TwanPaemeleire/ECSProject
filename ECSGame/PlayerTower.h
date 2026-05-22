#pragma once
#include <Component.h>
#include <unordered_set>

struct PlayerTower final : public Bloodforge::Component<PlayerTower>
{
	float Range = 200.0f;
	float Damage = 10.0f;
	float FireRateDelay = 0.02f;
	float TimeUntilNextShot = 0.0f;
	std::unordered_set<int> EnemiesToShoot;
};