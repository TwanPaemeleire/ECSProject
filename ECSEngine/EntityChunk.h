#pragma once
#include <tuple>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include "Entity.h"
#include <iostream>
#include <span>

struct IEntityChunk
{
	virtual ~IEntityChunk() = default;
	virtual bool IsFull() const = 0;
	virtual int AddEntity(int entityId) = 0;
	virtual void RemoveEntityAndComponents(Entity* entity) = 0;
	virtual bool ContainsComponent(int componentIndex) const = 0;
	virtual std::span<int> GetEntityIndices() const = 0;
	virtual void* GetComponentArray(int componentId) const = 0;
};

struct ComponentArray 
{
	void* Data;
	size_t ElementSize;
};

template <typename... Components> 
class EntityChunk : public IEntityChunk
{
	static_assert((std::is_default_constructible_v<Components> && ...), "All ECS components must be default constructible");

public:
	EntityChunk(size_t capacity);
	~EntityChunk() = default;

	bool IsFull() const override { return m_IsFull; }

	// Returns the index of the newly added entity within the chunk
	int AddEntity(int entityId) override;
	void RemoveEntityAndComponents(Entity* entity) override;

	void SetChunkIndex(int index) { m_ChunkIndex = index; }
	int GetChunkIndex() const { return m_ChunkIndex; }

	bool ContainsComponent(int componentIndex) const;
	std::span<int> GetEntityIndices() const;

	void* GetComponentArray(int componentId) const override;

private:
	void MoveComponents(int target, int source, const std::vector<int>& componentIndices);
	template<typename ComponentType>
	void ConstructComponentArray(size_t capacity);

	std::unique_ptr<int[]> m_EntityIds;
	std::unordered_map<int, ComponentArray> m_ComponentArrays;
	std::unordered_map<int, int> m_EntityIdToChunkIndex; // Map to track entity IDs and their corresponding chunk indices
	std::unordered_map<int, int> m_ComponentIdToArrayIndex; // Map to track component IDs and their corresponding array indices
	bool m_IsFull = false;
	size_t m_EntityCount = 0;
	size_t m_Capacity = 0;
	int m_ChunkIndex = -1;

	int m_CurrentFreeIndex = 0; // Index to keep track of the next free index in the chunk
};

template<typename ...Components>
inline EntityChunk<Components...>::EntityChunk(size_t capacity)
	: m_Capacity{ capacity }, m_EntityIds{ std::make_unique<int[]>(capacity) }
{
	int index = 0;
	((m_ComponentIdToArrayIndex[Components::Index] = index++), ...);
	((ConstructComponentArray<Components>(capacity)), ...);
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

	m_EntityIds[entityIndexInChunk] = entityId;
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
		m_EntityIds[entityIndexInChunk] = m_EntityIds[m_CurrentFreeIndex - 1];
		std::vector<int> indices = { ((Components::Index), ...) };
		MoveComponents(entityIndexInChunk, m_CurrentFreeIndex - 1, indices);
		m_EntityIdToChunkIndex[m_EntityIds[entityIndexInChunk]] = entityIndexInChunk; // Update the moved entity's index in the map
		// std::cout << "Moved entity with ID: " << m_EntityIds[entityIndexInChunk] << " to index: " << entityIndexInChunk << " originally at: " << m_CurrentFreeIndex - 1 << std::endl;
	}
	m_EntityIdToChunkIndex.erase(entity->GetId());
	--m_CurrentFreeIndex;
	--m_EntityCount;
	m_IsFull = false;
}

template<typename ...Components>
inline bool EntityChunk<Components...>::ContainsComponent(int componentIndex) const
{
	return m_ComponentIdToArrayIndex.contains(componentIndex);
}

template<typename ...Components>
inline std::span<int> EntityChunk<Components...>::GetEntityIndices() const
{
	return { m_EntityIds.get(), m_EntityCount};
}

template<typename ...Components>
inline void* EntityChunk<Components...>::GetComponentArray(int componentId) const
{
	auto it = m_ComponentArrays.find(componentId);
	if (it == m_ComponentArrays.end()) throw std::runtime_error("Component not found");
	return it->second.Data;
}

template<typename ...Components>
template<typename ComponentType>
inline void EntityChunk<Components...>::ConstructComponentArray(size_t capacity)
{
	ComponentType* array = new ComponentType[capacity];
	m_ComponentArrays[ComponentType::Index] =
	{
		static_cast<void*>(array),
		sizeof(ComponentType)
	};
}

template<typename ...Components>
inline void EntityChunk<Components...>::MoveComponents(int target, int source, const std::vector<int>& componentIndices)
{
	for(int id : componentIndices)
	{
		auto& array = m_ComponentArrays[id];
		char* data = static_cast<char*>(array.Data);
		std::memcpy(data + target * array.ElementSize,data + source * array.ElementSize,array.ElementSize);
	}
}