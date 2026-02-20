#pragma once
#include "Component.h"
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "EntityChunk.h"
#include <algorithm>
#include <string>

class EntityManager
{
public:
	EntityManager() = default;
	~EntityManager() = default;

	template <typename... Components>
	void CreateEntity(int newChunkCapacity = 20);

private:

	template <typename... Components>
	IEntityChunk* CreateNewChunk(std::string id, int capacity);

	template <typename T>
	std::type_index ReturnComponentInfo();

	std::unordered_map<std::string, std::vector<std::unique_ptr<IEntityChunk>>> m_EntityChunks;
	int m_CurrentEntityId = 0;
};

template<typename ...Components>
inline void EntityManager::CreateEntity(int newChunkCapacity)
{
	static_assert((std::is_base_of_v<Component, Components> && ...), "All types must be derived from Component");

	// TODO: Find a better way to create the signature
	// Create entity component combination signature
	std::vector<std::type_index> types = { std::type_index(typeid(Components))... };
	std::sort(types.begin(), types.end(), [](const std::type_index& a, const std::type_index& b) 
		{
			return a.hash_code() < b.hash_code();
		});
	std::string entitySignature = "";
	for(std::type_index var : types)
	{
		entitySignature += std::to_string(var.hash_code());
		 std::cout << "Creating entity with component type: " << var.hash_code() << std::endl;
	}

	IEntityChunk* chunk = nullptr;
	if (m_EntityChunks.find(entitySignature) == m_EntityChunks.end()) // Check if any chunk exists
	{
		chunk = CreateNewChunk<Components...>(entitySignature, newChunkCapacity);
	}
	else // Check if any chunk with space is left
	{
		for (const std::unique_ptr<IEntityChunk>& existingChunk : m_EntityChunks[entitySignature])
		{
			if (!existingChunk.get()->IsFull())
			{
				chunk = existingChunk.get();
				break;
			}
		}
		if(chunk == nullptr) // If all chunks are full, create a new one
		{
			chunk = CreateNewChunk<Components...>(entitySignature, newChunkCapacity);
		}
	}
	chunk->AddEntity(m_CurrentEntityId);
	++m_CurrentEntityId;
}

template<typename ...Components>
inline IEntityChunk* EntityManager::CreateNewChunk(std::string id, int capacity)
{
	std::unique_ptr<IEntityChunk> newChunk = std::make_unique<EntityChunk<Components...>>(capacity);
	IEntityChunk* chunkPtr = newChunk.get();
	m_EntityChunks[id].push_back(std::move(newChunk));
	std::cout << "Created new chunk for entity signature: " << id << std::endl;
	return chunkPtr;
}

template<typename T>
inline std::type_index EntityManager::ReturnComponentInfo()
{
	return std::type_index(typeid(T));
}