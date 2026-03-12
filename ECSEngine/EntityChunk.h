#pragma once
#include <tuple>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include "Entity.h"
#include <iostream>
#include <span>

struct ComponentArray 
{
	void* Data;
	size_t ElementSize;
};

class EntityChunk
{
public:
	EntityChunk(const ArchetypeIdentifier& identifier, size_t capacity);
	~EntityChunk();

	bool IsFull() const { return m_IsFull; }

	// Returns the index of the newly added entity within the chunk
	int AddEntity(int entityId);
	void RemoveEntityAndComponents(Entity* entity);

	void SetChunkIndex(int index) { m_ChunkIndex = index; }
	int GetChunkIndex() const { return m_ChunkIndex; }

	bool ContainsComponent(int componentIndex) const;
	std::span<int> GetEntityIndices() const;

	void* GetComponentArray(int componentId) const;

private:
	void MoveComponents(int target, int source, const std::vector<int>& componentIndices);
	void ConstructComponentArray(int componentIndex);

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