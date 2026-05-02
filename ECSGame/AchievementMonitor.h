#pragma once
#include <Singleton.h>

class AchievementMonitor final : public Singleton<AchievementMonitor>
{
public:
	void OnEnemyDeath();
};