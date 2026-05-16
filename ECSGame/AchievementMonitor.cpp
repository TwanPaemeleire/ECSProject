#include "AchievementMonitor.h"
#include <GlobalEventHandler.h>
#include <EntityManager.h>
#include <SteamUtils.h>
#include <SteamUnlockAchievementRequest.h>
#include <SteamIndicateAchievementProgressRequest.h>
#include <iostream>

using namespace Bloodforge;

void AchievementMonitor::OnEnemyDeath()
{
	if (SteamUtils::HasAchievementBeenUnlocked("ACH_TRAVEL_FAR_ACCUM")) return;
	float kills = 0;
	SteamUtils::GetStat("FeetTraveled", &kills);
	++kills;
	SteamUtils::SetStat("FeetTraveled", kills);
	// std::cout << "FeetTraveled : " << std::to_string(kills) << std::endl;
	EntityManager& entityManager = EntityManager::GetInstance();
	if (kills >= 5280)
	{
		Entity& achievementEntity = entityManager.CreateEntity();
		SteamUnlockAchievementRequest* requestComp = entityManager.AddComponent<SteamUnlockAchievementRequest>(achievementEntity);
		requestComp->AchievementId = "ACH_TRAVEL_FAR_ACCUM";
	}
	else if(static_cast<int>(kills) % 100 == 0)
	{
		Entity& progressIndicatorEntity = entityManager.CreateEntity();
		SteamIndicateAchievementProgressRequest* request = entityManager.AddComponent<SteamIndicateAchievementProgressRequest>(progressIndicatorEntity);
		request->AchievementId = "ACH_TRAVEL_FAR_ACCUM";
		request->CurrentProgress = 0;
		request->MaxProgess = 250;
	}
}