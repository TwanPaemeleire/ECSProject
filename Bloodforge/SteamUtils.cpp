#include "pch.h"
#include "SteamUtils.h"
#include <isteamuserstats.h>

bool Bloodforge::SteamUtils::GetStat(const char* statName, int* statValue)
{
    return SteamUserStats()->GetStat(statName, statValue);
}

bool Bloodforge::SteamUtils::GetStat(const char* statName, float* statValue)
{
    return SteamUserStats()->GetStat(statName, statValue);
}

bool Bloodforge::SteamUtils::SetStat(const char* statName, int statValue)
{
    return SteamUserStats()->SetStat(statName, statValue);
}

bool Bloodforge::SteamUtils::SetStat(const char* statName, float statValue)
{
    return SteamUserStats()->SetStat(statName, statValue);
}

bool Bloodforge::SteamUtils::HasAchievementBeenUnlocked(const char* statName)
{
    bool achieved = false;
    SteamUserStats()->GetAchievement(statName, &achieved);
    return achieved;
}
