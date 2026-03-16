#pragma once
#include "EntityManager.h"

template<typename Component>
inline void Entity::AddComponent()
{
	EntityManager::Instance.AddComponent<Component>(this);
}