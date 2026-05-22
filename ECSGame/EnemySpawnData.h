#pragma once
#include <Component.h>

struct EnemySpawnData final : public Bloodforge::Component<EnemySpawnData>
{
	float SpawnRadius = 400.0f;
	float EnemySpeed = 40.0f;
	float SpawnInterval = 0.02f;
};