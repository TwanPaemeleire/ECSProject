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
#include "ComponentRegistry.h"

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

	static EntityManager Instance;

	EntityManager();
	~EntityManager() = default;

	template <typename... Components>
	Entity* CreateEntity(int newChunkCapacity = 20);
	
	void DestroyEntity(int entityId);
	void DestroyEntity(const std::vector<int>& entityIds);
	void DestroyEntity(Entity* entity);

	Entity* GetEntity(int entityId) const { return m_Entities.at(entityId).get(); }
	ComponentRegistry* GetComponentRegistry() const { return m_ComponentRegistry.get(); }

	template <typename... Components>
	EntityQueryResult<Components...> QueryEntities();

	template<typename ComponentType>
	void AddComponent(Entity* entity);
private:

	template <typename... ComponentTypes>
	ArchetypeIdentifier GetArchetypeIds();
	template <typename ComponentType>
	void UpdateArchetypeId(Entity* entity, bool isAdd = true);

	EntityChunk* GetFirstAvailableChunk(const ArchetypeIdentifier& id, int capacity);
	EntityChunk* CreateNewChunk(const ArchetypeIdentifier& id, int capacity);

	template <typename T>
	std::type_index ReturnComponentInfo();

	std::unordered_map<ArchetypeIdentifier, std::vector<std::unique_ptr<EntityChunk>>, ArchetypeIdentifierHash> m_EntityChunks;
	std::unordered_map<int, std::unique_ptr<Entity>> m_Entities;
	std::unique_ptr<ComponentRegistry> m_ComponentRegistry;
	int m_CurrentEntityId = 0;

	// int m_ComponentIndexTracker = 0;
};

template<typename ...Components>
inline Entity* EntityManager::CreateEntity(int newChunkCapacity)
{
	// static_assert((std::is_base_of_v<Component, Components> && ...), "All types must be derived from Component");
	//std::cout << "---------- Creating Entity ----------" << std::endl;

	ArchetypeIdentifier identifier = GetArchetypeIds<Components...>();
	EntityChunk* chunk = GetFirstAvailableChunk(identifier, newChunkCapacity);

	std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(m_CurrentEntityId);
	Entity* newEntityPtr = newEntity.get();
	chunk->AddEntity(m_CurrentEntityId);
	newEntity->SetCurrentChunk(identifier, static_cast<int>(m_EntityChunks[identifier].size()) - 1);
	m_Entities[m_CurrentEntityId] = std::move(newEntity);
	++m_CurrentEntityId;

	//std::cout << "-------------------------------------" << std::endl;
	return newEntityPtr;
}

template<typename ComponentType>
inline void EntityManager::AddComponent(Entity* entity)
{
	auto* oldChunk = m_EntityChunks[entity->GetCurrentArchetypeIds()][entity->GetCurrentChunkIndex()].get();
	UpdateArchetypeId<ComponentType>(entity);
	// auto* newChunk = GetFirstAvailableChunk<oldchunk.template..., ComponentType>();
}

template<typename ...Components>
inline EntityQueryResult<Components...> EntityManager::QueryEntities()
{
	EntityQueryResult<Components...> result;

	for (const auto& pair : m_EntityChunks)
	{
		if ((pair.second[0].get()->ContainsComponent(Component<Components>::Index) && ...))
		{
			for (const std::unique_ptr<EntityChunk>& chunk : pair.second)
			{
				ChunkView<Components...> chunkView;
				chunkView.Entities = chunk->GetEntityIndices();
				chunkView.ComponentArrays = std::make_tuple(std::span<Components>(static_cast<Components*>(chunk->GetComponentArray(Component<Components>::Index)),chunk->GetEntityIndices().size())...);
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

template<typename ...ComponentTypes>
inline ArchetypeIdentifier EntityManager::GetArchetypeIds()
{
	// if (((Component<ComponentTypes>::Index == -1) || ...))
	// {
	// 	((Component<ComponentTypes>::Index == -1 ? Component<ComponentTypes>::Index = m_ComponentIndexTracker++ : 0), ...);
	// }
	((m_ComponentRegistry->TryRegisterComponent<ComponentTypes>()), ...);

	ArchetypeIdentifier ids = { Component<ComponentTypes>::Index... };
	std::sort(ids.begin(), ids.end());
	return ids;
}

template<typename ComponentType>
inline void EntityManager::UpdateArchetypeId(Entity* entity, bool isAdd)
{
	if (isAdd)
	{
		ArchetypeIdentifier& archetypeId = entity->GetCurrentArchetypeIds();
		archetypeId.emplace_back(Component<ComponentType>::Index);
		std::sort(archetypeId.begin(), archetypeId.end());
	}
	else
	{
		ArchetypeIdentifier& archetypeId = entity->GetCurrentArchetypeIds();
		archetypeId.erase(Component<ComponentType>::Index);
	}
}