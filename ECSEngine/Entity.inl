#pragma once
#include "EntityManager.h"

template<typename Component>
inline void Entity::AddComponent()
{
	// Tell EntityManager to move which chunk this entity is in
	EntityManager::Instance.AddComponent<Component>(this);
}