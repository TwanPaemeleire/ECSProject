#pragma once
#include <Component.h>

struct SceneSwitchDataHolder final : public Bloodforge::Component<SceneSwitchDataHolder>
{
	int NextSceneId = -1;
	int PreviousSceneId = -1;
};