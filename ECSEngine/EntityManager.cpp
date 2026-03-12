#include "pch.h"
#include "EntityManager.h"
#include <iostream>
#include "EntityChunk.h"

EntityManager EntityManager::Instance = EntityManager{};

EntityManager::EntityManager()
	:m_ComponentRegistry{std::make_unique<ComponentRegistry>()}
{
}

void EntityManager::DestroyEntity(int entityId)
{
	//std::cout << "--------- Destroying Entity ---------" << std::endl;
	Entity* entity = m_Entities[entityId].get();
	if (entity)
	{
		const ArchetypeIdentifier& chunkId = entity->GetCurrentArchetypeIds();
		int chunkIndex = entity->GetCurrentChunkIndex();
		EntityChunk* entityChunk = m_EntityChunks[chunkId][chunkIndex].get();
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

EntityChunk* EntityManager::GetFirstAvailableChunk(const ArchetypeIdentifier& id, int capacity)
{
	EntityChunk* chunk = nullptr;
	if (m_EntityChunks.find(id) == m_EntityChunks.end()) // Check if any chunk exists
	{
		chunk = CreateNewChunk(id, capacity);
	}
	else // Check if any chunk with space is left
	{
		for (const std::unique_ptr<EntityChunk>& existingChunk : m_EntityChunks[id])
		{
			if (!existingChunk.get()->IsFull())
			{
				chunk = existingChunk.get();
				//std::cout << "Existing chunk with space was found" << std::endl;
				break;
			}
		}
		if (chunk == nullptr) // If all chunks are full, create a new one
		{
			chunk = CreateNewChunk(id, capacity);
		}
	}
	return chunk;
}

EntityChunk* EntityManager::CreateNewChunk(const ArchetypeIdentifier& id, int capacity)
{
	std::unique_ptr<EntityChunk> newChunk = std::make_unique<EntityChunk>(id, capacity);
	EntityChunk* chunkPtr = newChunk.get();
	m_EntityChunks[id].push_back(std::move(newChunk));
	chunkPtr->SetChunkIndex(static_cast<int>(m_EntityChunks[id].size()) - 1);
	//std::cout << "Created new chunk for entity signature: " << id << std::endl;
	return chunkPtr;
}
