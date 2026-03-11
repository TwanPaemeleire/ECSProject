#pragma once
#include "Component.h"
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "EntityChunk.h"
#include <string>
#include <algorithm>
#include "Entity.h"
#include <span>

class Entity;

template <typename... Components>
struct ChunkView
{
	std::span<int> Entities;
	std::tuple<std::span<Components>...> ComponentArrays;
};

template <typename... Components>
struct EntityQueryResult 
{
	std::vector<ChunkView<Components...>> Chunks;
};

class EntityManager
{
public:
	EntityManager() = default;
	~EntityManager() = default;

	template <typename... Components>
	Entity* CreateEntity(int newChunkCapacity = 20);
	
	void DestroyEntity(int entityId);
	void DestroyEntity(const std::vector<int>& entityIds);
	void DestroyEntity(Entity* entity);

	Entity* GetEntity(int entityId) const { return m_Entities.at(entityId).get(); }

	template <typename... Components>
	EntityQueryResult<Components...> QueryEntities();

private:

	template <typename... Components>
	IEntityChunk* CreateNewChunk(const std::string& id, int capacity);

	template <typename T>
	std::type_index ReturnComponentInfo();

	std::unordered_map<std::string, std::vector<std::unique_ptr<IEntityChunk>>> m_EntityChunks;
	std::unordered_map<int, std::unique_ptr<Entity>> m_Entities;
	int m_CurrentEntityId = 0;

	int m_ComponentIndexTracker = 0;
};

template<typename ...Components>
inline Entity* EntityManager::CreateEntity(int newChunkCapacity)
{
	// static_assert((std::is_base_of_v<Component, Components> && ...), "All types must be derived from Component");
	//std::cout << "---------- Creating Entity ----------" << std::endl;

	// TODO: Find a better way to create the signature
	// Requirements: - The same combination of components should always result in the same signature, regardless of the order of the components
	// 				 - Different combinations of components should result in different signatures
	//               - We should be able to easily map a component index to its position in the chunk (for example, if we have a chunk with TransformComponent and SpriteComponent, we should be able to easily find the index of the TransformComponent in the chunk)
	//	             - We should be able to remove/add a component from an entity and then recalculate the signature
	std::string id = "";
	if (((Component<Components>::Index == -1) || ...))
	{
		((Component<Components>::Index == -1 ? Component<Components>::Index = m_ComponentIndexTracker++ : 0), ...);
	}
	std::vector<int> indices = { Component<Components>::Index... };
	std::sort(indices.begin(), indices.end());
	id.reserve(indices.size() * 3);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		if (i > 0) id.push_back(',');
		id += std::to_string(indices[i]);
	}

	// Chunk holds a map of component index to component array index, so we can easily query

	IEntityChunk* chunk = nullptr;
	if (m_EntityChunks.find(id) == m_EntityChunks.end()) // Check if any chunk exists
	{
		chunk = CreateNewChunk<Components...>(id, newChunkCapacity);
	}
	else // Check if any chunk with space is left
	{
		for (const std::unique_ptr<IEntityChunk>& existingChunk : m_EntityChunks[id])
		{
			if (!existingChunk.get()->IsFull())
			{
				chunk = existingChunk.get();
				//std::cout << "Existing chunk with space was found" << std::endl;
				break;
			}
		}
		if(chunk == nullptr) // If all chunks are full, create a new one
		{
			chunk = CreateNewChunk<Components...>(id, newChunkCapacity);
		}
	}
	std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(m_CurrentEntityId);
	Entity* newEntityPtr = newEntity.get();
	chunk->AddEntity(m_CurrentEntityId);
	newEntity->SetCurrentChunk(id, static_cast<int>(m_EntityChunks[id].size()) - 1);
	m_Entities[m_CurrentEntityId] = std::move(newEntity);
	++m_CurrentEntityId;

	//std::cout << "-------------------------------------" << std::endl;
	return newEntityPtr;
}

template<typename ...Components>
inline IEntityChunk* EntityManager::CreateNewChunk(const std::string& id, int capacity)
{
	std::unique_ptr<EntityChunk<Components...>> newChunk = std::make_unique<EntityChunk<Components...>>(capacity);
	EntityChunk<Components...>* chunkPtr = newChunk.get();
	m_EntityChunks[id].push_back(std::move(newChunk));
	chunkPtr->SetChunkIndex(static_cast<int>(m_EntityChunks[id].size()) - 1);
	//std::cout << "Created new chunk for entity signature: " << id << std::endl;
	return chunkPtr;
}

template<typename ...Components>
inline EntityQueryResult<Components...> EntityManager::QueryEntities()
{
	EntityQueryResult<Components...> result;

	for (const auto& pair : m_EntityChunks)
	{
		if ((pair.second[0].get()->ContainsComponent(Component<Components>::Index) && ...))
		{
			for (const std::unique_ptr<IEntityChunk>& chunk : pair.second)
			{
				EntityChunk<Components...>* castedChunk = static_cast<EntityChunk<Components...>*>(chunk.get());
				ChunkView<Components...> chunkView;
				chunkView.Entities = castedChunk->GetEntityIndices();
				chunkView.ComponentArrays = std::make_tuple(castedChunk->GetComponentArray<Components>()...);
				result.Chunks.push_back(chunkView);
			}
		}
	}
	return result;
}

template<typename T>
inline std::type_index EntityManager::ReturnComponentInfo()
{
	return std::type_index(typeid(T));
}