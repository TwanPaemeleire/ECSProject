#pragma once
#include "Component.h"
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "EntityChunk.h"
#include <iostream>

class EntityManager
{
public:
	EntityManager() = default;
	~EntityManager() = default;

	template <typename... Components>
	void CreateEntity(int newChunkCapacity = 20);

private:

	template <typename... Components>
	void CreateNewChunk(std::uint64_t id, int capacity);

	template <typename T>
	std::type_index ReturnComponentInfo();

	std::unordered_map<std::uint64_t, std::unique_ptr<IEntityChunk>> m_EntityChunks;
	int m_CurrentEntityId = 0;
};

template<typename ...Components>
inline void EntityManager::CreateEntity(int newChunkCapacity)
{
	static_assert((std::is_base_of_v<Component, Components> && ...), "All types must be derived from Component");

	std::vector<std::type_index> types = { std::type_index(typeid(Components))... };
	for(std::type_index var : types)
	{
		std::cout << "Creating entity with component type: " << var.hash_code() << std::endl;
	}

	int tempChunkId = 20;
	IEntityChunk* chunk = nullptr;
	if (m_EntityChunks.find(tempChunkId) == m_EntityChunks.end())
	{
		CreateNewChunk<Components...>(tempChunkId, newChunkCapacity);
	}
	chunk = m_EntityChunks[tempChunkId].get();
	chunk->AddEntity(m_CurrentEntityId);
	++m_CurrentEntityId;
}

template<typename ...Components>
inline void EntityManager::CreateNewChunk(std::uint64_t id, int capacity)
{
	m_EntityChunks[id] = std::make_unique<EntityChunk<Components...>>(capacity);
}

template<typename T>
inline std::type_index EntityManager::ReturnComponentInfo()
{
	return std::type_index(typeid(T));
}