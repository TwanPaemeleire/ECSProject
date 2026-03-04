#include "pch.h"
#include "EntityManager.h"
#include <iostream>
#include "EntityChunk.h"

void EntityManager::DestroyEntity(int entityId)
{
	//std::cout << "--------- Destroying Entity ---------" << std::endl;
	Entity* entity = m_Entities[entityId].get();
	if (entity)
	{
		const std::string& chunkId = entity->GetCurrentChunkId();
		int chunkIndex = entity->GetCurrentChunkIndex();
		IEntityChunk* entityChunk = m_EntityChunks[chunkId][chunkIndex].get();
		entityChunk->RemoveEntityAndComponents(entity);
		m_Entities.erase(entityId);
	}
	else
	{
		//std::cout << "No entity found with ID: " << entityId << std::endl;
	}
	//std::cout << "-------------------------------------" << std::endl;
}

void EntityManager::DestroyEntity(const std::vector<int>& entityIds)
{
	for (int entityId : entityIds)
	{
		DestroyEntity(entityId);
	}
}

void EntityManager::DestroyEntity(Entity* entity)
{
	DestroyEntity(entity->GetId());
}
