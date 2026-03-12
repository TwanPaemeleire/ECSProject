#include "pch.h"
#include "EntityChunk.h"
#include "EntityManager.h"

EntityChunk::EntityChunk(const ArchetypeIdentifier& identifier, size_t capacity)
	: m_Capacity{ capacity }, m_EntityIds{ std::make_unique<int[]>(capacity) }
{
	// ((m_ComponentIdToArrayIndex[Components::Index] = index++), ...);
	// ((ConstructComponentArray<Components>(capacity)), ...);
	int index = 0;
	for (int id : identifier)
	{
		m_ComponentIdToArrayIndex[id] = index++;
		ConstructComponentArray(id);
	}

	// std::cout << "EntityChunk created with capacity: " << capacity << std::endl;
	// std::cout << "With following component types: ";
	// ((std::cout << typeid(Components).name() << " "), ...);
	// std::cout << std::endl;
}

EntityChunk::~EntityChunk()
{
	for (auto& [id, array] : m_ComponentArrays)
	{
		std::free(array.Data);
	}
}

int EntityChunk::AddEntity(int entityId)
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

void EntityChunk::RemoveEntityAndComponents(Entity* entity)
{
	int entityIndexInChunk = m_EntityIdToChunkIndex[entity->GetId()];
	if (entityIndexInChunk == m_CurrentFreeIndex - 1) // If the entity to be removed is the last one, we can simply clear it without moving any data
	{
		// std::cout << "Removed entity with ID: " << entity->GetId() << " from index: " << entityIndexInChunk << std::endl;
		// std::cout << "No data needed to be moved" << std::endl;
	}
	else
	{
		m_EntityIds[entityIndexInChunk] = m_EntityIds[m_CurrentFreeIndex - 1];
		MoveComponents(entityIndexInChunk, m_CurrentFreeIndex - 1, entity->GetCurrentArchetypeIds());

		m_EntityIdToChunkIndex[m_EntityIds[entityIndexInChunk]] = entityIndexInChunk; // Update the moved entity's index in the map
		// std::cout << "Moved entity with ID: " << m_EntityIds[entityIndexInChunk] << " to index: " << entityIndexInChunk << " originally at: " << m_CurrentFreeIndex - 1 << std::endl;
	}
	m_EntityIdToChunkIndex.erase(entity->GetId());
	--m_CurrentFreeIndex;
	--m_EntityCount;
	m_IsFull = false;
}

bool EntityChunk::ContainsComponent(int componentIndex) const
{
	return m_ComponentIdToArrayIndex.contains(componentIndex);
}

std::span<int> EntityChunk::GetEntityIndices() const
{
	return { m_EntityIds.get(), m_EntityCount };
}

void* EntityChunk::GetComponentArray(int componentId) const
{
	auto it = m_ComponentArrays.find(componentId);
	if (it == m_ComponentArrays.end()) throw std::runtime_error("Component not found");
	return it->second.Data;
}


void EntityChunk::ConstructComponentArray(int componentIndex)
{
	auto& info = EntityManager::Instance.GetComponentRegistry()->GetComponentInfo(componentIndex);
	void* arrayMemory = std::malloc(info.Size * m_Capacity);

	m_ComponentArrays[componentIndex] =
	{
		arrayMemory,
		info.Size
	};
}

void EntityChunk::MoveComponents(int target, int source, const std::vector<int>& componentIndices)
{
	for (int id : componentIndices)
	{
		auto& array = m_ComponentArrays[id];
		char* data = static_cast<char*>(array.Data);
		std::memcpy(data + target * array.ElementSize, data + source * array.ElementSize, array.ElementSize);
	}
}