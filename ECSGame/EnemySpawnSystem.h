#pragma once
#include <System.h>
#include <random>

struct Enemy;

class EnemySpawnSystem final : public Bloodforge::System
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;
private:
	void SpawnLoop();
	void SpawnEnemy();
	void InitializeEnemy1(Enemy* enemy);

	float m_SpawnRadius = 400.0f;
};