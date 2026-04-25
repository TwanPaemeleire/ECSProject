#include "pch.h"
#include "EntityManager.h"
#include <iostream>
#include "EntityChunk.h"
namespace Bloodforge
{
	EntityManager::EntityManager()
		:m_ComponentRegistry{ std::make_unique<ComponentRegistry>() }
	{
	}

	void EntityManager::DestroyEntity(int entityId)
	{
		//std::cout << "--------- Destroying Entity ---------" << std::endl;
		Entity& entity = m_Entities[entityId];
		entity.MarkedForDestruction = true;
		m_EntitiesToDestroy.emplace_back(entityId);
		//std::cout << "-------------------------------------" << std::endl;
	}

	void EntityManager::DestroyEntity(const std::vector<int>& entityIds)
	{
		for (int entityId : entityIds)
		{
			DestroyEntity(entityId);
		}
	}

	void EntityManager::DestroyEntity(Entity& entity)
	{
		DestroyEntity(entity.Id);
	}

	void EntityManager::DestroyAllEntities()
	{
		m_Entities.clear();
		m_EntityChunks.clear();
		m_FreeIndices.clear();
	}

	void EntityManager::DestroyMarkedForDestructionEntities()
	{
		// for (int entityId : m_EntitiesToDestroy)
		// {
		// 	Entity& entity = GetEntity(entityId);
		// 	const ArchetypeIdentifierMask& chunkId = entity.CurrentArchetypeId;
		// 	int chunkIndex = entity.CurrentChunkIndex;
		// 	EntityChunk* entityChunk = m_EntityChunks[chunkId][chunkIndex].get();
		// 	entityChunk->RemoveEntityAndComponents(entity);
		// 	m_FreeIndices.push_back(entityId);
		// }

		std::for_each(m_EntitiesToDestroy.begin(), m_EntitiesToDestroy.end(), [this](int entityId)
			{
				Entity& entity = GetEntity(entityId);
				entity.IsAlive = false;
				const ArchetypeIdentifierMask& chunkId = entity.CurrentArchetypeId;
				int chunkIndex = entity.CurrentChunkIndex;
				EntityChunk* entityChunk = m_EntityChunks[chunkId][chunkIndex].get();
				entityChunk->RemoveEntityAndComponents(entity);
				m_FreeIndices.push_back(entityId);
			});

		m_EntitiesToDestroy.clear();
	}

	EntityChunk* EntityManager::GetFirstAvailableChunk(ArchetypeIdentifierMask& id, int capacity)
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
				if (!existingChunk->IsFull())
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

	EntityChunk* EntityManager::CreateNewChunk(ArchetypeIdentifierMask& id, int capacity)
	{
		std::unique_ptr<EntityChunk> newChunk = std::make_unique<EntityChunk>(id, capacity);
		EntityChunk* chunkPtr = newChunk.get();
		m_EntityChunks[id].push_back(std::move(newChunk));
		chunkPtr->SetChunkIndex(static_cast<int>(m_EntityChunks[id].size()) - 1);
		//std::cout << "Created new chunk for entity signature: " << id << std::endl;
		return chunkPtr;
	}
}