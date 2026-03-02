#pragma once
#include "Component.h"
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "EntityChunk.h"
#include <string>
#include <algorithm>
#include "Entity.h"

class Entity;

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

private:

	template <typename... Components>
	IEntityChunk* CreateNewChunk(const std::string& id, int capacity);

	template <typename T>
	std::type_index ReturnComponentInfo();

	std::unordered_map<std::string, std::vector<std::unique_ptr<IEntityChunk>>> m_EntityChunks;
	std::unordered_map<int, std::unique_ptr<Entity>> m_Entities;
	int m_CurrentEntityId = 0;
};

template<typename ...Components>
inline Entity* EntityManager::CreateEntity(int newChunkCapacity)
{
	static_assert((std::is_base_of_v<Component, Components> && ...), "All types must be derived from Component");
	std::cout << "---------- Creating Entity ----------" << std::endl;

	// TODO: Find a better way to create the signature
	// Create entity component combination signature
	std::vector<std::type_index> types = { std::type_index(typeid(Components))... };
	std::sort(types.begin(), types.end(), [](const std::type_index& a, const std::type_index& b) 
		{
			return a.hash_code() < b.hash_code();
		});
	std::string entitySignature = "";

	std::cout << "Creating entity with component types: ";
	for(std::type_index var : types)
	{
		entitySignature += std::to_string(var.hash_code());
		std::cout << var.name() << " ";
	}
	std::cout << std::endl;

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
				std::cout << "Existing chunk with space was found" << std::endl;
				break;
			}
		}
		if(chunk == nullptr) // If all chunks are full, create a new one
		{
			chunk = CreateNewChunk<Components...>(entitySignature, newChunkCapacity);
		}
	}
	std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(m_CurrentEntityId);
	Entity* newEntityPtr = newEntity.get();
	chunk->AddEntity(m_CurrentEntityId);
	newEntity->SetCurrentChunk(entitySignature, static_cast<int>(m_EntityChunks[entitySignature].size()) - 1);
	m_Entities[m_CurrentEntityId] = std::move(newEntity);
	++m_CurrentEntityId;

	std::cout << "-------------------------------------" << std::endl;
	return newEntityPtr;
}

template<typename ...Components>
inline IEntityChunk* EntityManager::CreateNewChunk(const std::string& id, int capacity)
{
	std::unique_ptr<EntityChunk<Components...>> newChunk = std::make_unique<EntityChunk<Components...>>(capacity);
	EntityChunk<Components...>* chunkPtr = newChunk.get();
	m_EntityChunks[id].push_back(std::move(newChunk));
	chunkPtr->SetChunkIndex(static_cast<int>(m_EntityChunks[id].size()) - 1);
	std::cout << "Created new chunk for entity signature: " << id << std::endl;
	return chunkPtr;
}

template<typename T>
inline std::type_index EntityManager::ReturnComponentInfo()
{
	return std::type_index(typeid(T));
}