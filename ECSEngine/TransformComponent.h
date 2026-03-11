#pragma once
#include "Component.h"

struct TransformComponent : Component<TransformComponent>
{
public:
	TransformComponent() = default;
	~TransformComponent() = default;
	int TestVar = 0;
};

