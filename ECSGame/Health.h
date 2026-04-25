#pragma once
#include <Component.h>
#include "Event.h"

struct Health final : public Bloodforge::Component<Health>
{
	float MaxHealth = 100.0f;
	float CurrentHealth = 100.0f;
	Bloodforge::Event<float> OnHealthChangedEvent;
	Bloodforge::Event<int> OnDeathEvent;
};