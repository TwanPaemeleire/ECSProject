#pragma once
#include <tuple>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Entity.h"
#include <ranges>
//#include "EntityManager.h"

struct IEntityChunk
{
	virtual ~IEntityChunk() = default;
	virtual bool IsFull() const = 0;
	virtual int AddEntity(int entityId) = 0;
	virtual void RemoveEntityAndComponents(Entity* entity) = 0;
};

template <typename... Components> 
class EntityChunk : public IEntityChunk
{
public:
	EntityChunk(size_t capacity);
	~EntityChunk() = default;

	bool IsFull() const override { return m_IsFull; }

	// Returns the index of the newly added entity within the chunk
	int AddEntity(int entityId) override;
	void RemoveEntityAndComponents(Entity* entity) override;

	void SetChunkIndex(int index) { m_ChunkIndex = index; }
	int GetChunkIndex() const { return m_ChunkIndex; }

private:
	template<std::size_t... indexSequence>
	void ConstructComponents(std::index_sequence<indexSequence...>); // std::index_sequence is just a compile-time sequence of integers {0, 1, 2, 3, etc.}
	template<std::size_t... indexSequence>
	void MoveComponents(int target, int source, std::index_sequence<indexSequence...>);

	std::tuple<std::unique_ptr<int[]>,std::unique_ptr<Components[]>...> m_Data; // First array is for entity IDs, followed by arrays for each component type
	std::unordered_map<int, int> m_EntityIdToChunkIndex; // Map to track entity IDs and their corresponding chunk indices
	bool m_IsFull = false;
	int m_EntityCount = 0;
	size_t m_Capacity = 0;
	int m_ChunkIndex = -1;

	int m_CurrentFreeIndex = 0; // Index to keep track of the next free index in the chunk
};

template<typename ...Components>
inline EntityChunk<Components...>::EntityChunk(size_t capacity)
	: m_Capacity{ capacity }, m_Data{ std::make_unique<int[]>(capacity), std::make_unique<Components[]>(capacity)... }
{
	 // std::cout << "EntityChunk created with capacity: " << capacity << std::endl;
	 // std::cout << "With following component types: ";
	 // ((std::cout << typeid(Components).name() << " "), ...);
	 // std::cout << std::endl;
}

template<typename ...Components>
inline int EntityChunk<Components...>::AddEntity(int entityId)
{
	if (IsFull())
	{
		throw std::runtime_error("Chunk is full. Cannot add more entities.");
	}

	int entityIndexInChunk = m_CurrentFreeIndex;

	 // std::cout << "Adding entity with ID: " << entityId << " to chunk with components:";
	 // ((std::cout << " " << typeid(Components).name()), ...);
	 // std::cout << std::endl << "Chunk fill progress: " << m_EntityCount + 1 << "/" << m_Capacity << std::endl;

	std::get<0>(m_Data)[entityIndexInChunk] = entityId;
	ConstructComponents(std::index_sequence_for<Components...>{});
	m_EntityIdToChunkIndex[entityId] = entityIndexInChunk;

	++m_EntityCount;
	++m_CurrentFreeIndex;
	m_IsFull = (m_EntityCount) >= m_Capacity;
	return entityIndexInChunk; // Return the index of the newly added entity
}

template<typename ...Components>
inline void EntityChunk<Components...>::RemoveEntityAndComponents(Entity* entity)
{
	int entityIndexInChunk = m_EntityIdToChunkIndex[entity->GetId()];
	if(entityIndexInChunk == m_Capacity - 1 || entityIndexInChunk == m_CurrentFreeIndex - 1) // If the entity to be removed is the last one, we can simply clear it without moving any data
	{
		// std::cout << "Removed entity with ID: " << entity->GetId() << " from index: " << entityIndexInChunk << std::endl;
		// std::cout << "No data needed to be moved" << std::endl;
	}
	else 
	{
		std::get<0>(m_Data)[entityIndexInChunk] = std::get<0>(m_Data)[m_CurrentFreeIndex - 1];
		MoveComponents(entityIndexInChunk, m_CurrentFreeIndex - 1, std::index_sequence_for<Components...>());
		m_EntityIdToChunkIndex[std::get<0>(m_Data)[entityIndexInChunk]] = entityIndexInChunk; // Update the moved entity's index in the map
		// std::cout << "Moved entity with ID: " << std::get<0>(m_Data)[entityIndexInChunk] << " to index: " << entityIndexInChunk << " originally at: " << m_CurrentFreeIndex - 1 << std::endl;
	}
	m_EntityIdToChunkIndex.erase(entity->GetId());
	--m_CurrentFreeIndex;
	--m_EntityCount;
	m_IsFull = false;
}

template<typename ...Components>
template<std::size_t ...indexSequence>
inline void EntityChunk<Components...>::ConstructComponents(std::index_sequence<indexSequence...>)
{
	((std::get<indexSequence + 1>(m_Data)[m_EntityCount] = Components{}),...);
}

template<typename ...Components>
template<std::size_t ...indexSequence>
inline void EntityChunk<Components...>::MoveComponents(int target, int source, std::index_sequence<indexSequence...>)
{
	((std::get<indexSequence + 1>(m_Data)[target] = std::get<indexSequence + 1>(m_Data)[source]), ...);
}