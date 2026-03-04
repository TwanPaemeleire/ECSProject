#pragma once
#include "Component.h"

struct SpriteComponent : public Component<SpriteComponent>
{
public:
	SpriteComponent() = default;
	~SpriteComponent() = default;
};