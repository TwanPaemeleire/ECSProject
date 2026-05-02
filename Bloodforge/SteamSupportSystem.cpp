#include "pch.h"
#include "SteamSupportSystem.h"
#include <steam_api.h>
#include <iostream>
#include "EntityManager.h"

#include "SteamUnlockAchievementRequest.h"
#include "SteamResetUserDataRequest.h"
#include "SteamIndicateAchievementProgressRequest.h"
#include "FunctionInvokeComponent.h"

#include "SteamUtils.h"

using namespace Bloodforge;

SteamSupportSystem::SteamSupportSystem()
	: m_CallbackAchievementStored(this, &SteamSupportSystem::OnAchievementStored),
	  m_CallbackUserStatsStored(this, &SteamSupportSystem::OnUserStatsStored)
{

}

void SteamSupportSystem::OnStart()
{
	SteamErrMsg errMsg = { 0 };
	ESteamAPIInitResult result = SteamAPI_InitEx(&errMsg);
	std::cout << "Steam API Init Result: " << result << std::endl;
	if (result != k_ESteamAPIInitResult_OK)
	{
		std::cerr << "Failed to initialize Steam API: " << errMsg << std::endl;
		return;
	}
	m_IsSteamInitialized = true;

	Entity& resetUserDataEntity = EntityManager::GetInstance().CreateEntity();;
	EntityManager::GetInstance().AddComponent<SteamResetUserDataRequest>(resetUserDataEntity);
}

void SteamSupportSystem::OnUpdate()
{
	SteamAPI_RunCallbacks();
	CheckUnlockAchievementRequests();
	CheckResetAllAchievementsRequests();
	CheckIndicateAchievementProgressRequests();
}

void Bloodforge::SteamSupportSystem::CheckUnlockAchievementRequests()
{
	auto& entityManager = EntityManager::GetInstance();
	EntityQueryResult<SteamUnlockAchievementRequest> result = entityManager.QueryEntities<SteamUnlockAchievementRequest>();
	for (EntityView<SteamUnlockAchievementRequest>& view : result.EntityViews)
	{
		SteamUserStats()->SetAchievement(view.GetComponent<SteamUnlockAchievementRequest>().AchievementId.c_str());
		SteamUserStats()->StoreStats();
		EntityManager::GetInstance().DestroyEntity(view.EntityId);
	}
}

void Bloodforge::SteamSupportSystem::CheckResetAllAchievementsRequests()
{
	auto& entityManager = EntityManager::GetInstance();
	EntityQueryResult<SteamResetUserDataRequest> result = entityManager.QueryEntities<SteamResetUserDataRequest>();
	for (EntityView<SteamResetUserDataRequest>& view : result.EntityViews)
	{
		SteamUserStats()->ResetAllStats( view.GetComponent<SteamResetUserDataRequest>().AlsoResetAchievements );
		EntityManager::GetInstance().DestroyEntity(view.EntityId);
	}
}

void Bloodforge::SteamSupportSystem::CheckIndicateAchievementProgressRequests()
{
	auto& entityManager = EntityManager::GetInstance();
	EntityQueryResult<SteamIndicateAchievementProgressRequest> result = entityManager.QueryEntities<SteamIndicateAchievementProgressRequest>();
	for (EntityView<SteamIndicateAchievementProgressRequest>& view : result.EntityViews)
	{
		SteamIndicateAchievementProgressRequest& request = view.GetComponent<SteamIndicateAchievementProgressRequest>();
		SteamUserStats()->IndicateAchievementProgress(request.AchievementId.c_str(), request.CurrentProgress, request.MaxProgess);
		EntityManager::GetInstance().DestroyEntity(view.EntityId);
	}
}

void SteamSupportSystem::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	std::cout << "Achievement stored callback received for achievement: " << pCallback->m_rgchAchievementName << std::endl;
}

void SteamSupportSystem::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	if (k_EResultOK == pCallback->m_eResult)
	{
		std::cout << "User stats stored successfully on Steam" << std::endl;
	}
	else
	{
		if (SteamUser()->BLoggedOn())
		{
			char buffer[128];
			_snprintf(buffer, 128, "UserStatsStored - failed, %d\n", pCallback->m_eResult);
			std::cout << buffer << std::endl;
		}
		else
		{
			std::cout << "User stats stored - offline mode, not syncing with Steam" << std::endl;
		}
	}
}