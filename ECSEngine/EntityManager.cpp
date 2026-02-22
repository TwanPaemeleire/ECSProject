#include "pch.h"
#include "EntityManager.h"
#include <iostream>
#include "EntityChunk.h"

void EntityManager::DestroyEntity(int entityId)
{
	Entity* entity = m_Entities[entityId].get();
	if (entity)
	{
		std::string chunkId = entity->GetCurrentChunkId();
		int chunkIndex = entity->GetCurrentChunkIndex();
		IEntityChunk* entityChunk = m_EntityChunks[chunkId][chunkIndex].get();
		entityChunk->RemoveEntityAndComponents(entity->GetCurrentChunkEntityIndex());
		m_Entities.erase(entityId);
	}
}

void EntityManager::DestroyEntity(const std::vector<int>& entityIds)
{
}

void EntityManager::DestroyEntity(Entity* entity)
{
}
