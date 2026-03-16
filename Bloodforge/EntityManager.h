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
#include "ArchetypeIdentifierMask.h"
#include "Singleton.h"

namespace Bloodforge
{
	struct Entity;

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

	class EntityManager : public Singleton<EntityManager>
	{
	public:
		~EntityManager() = default;

		template <typename... Components>
		Entity& CreateEntity(int newChunkCapacity = 20);

		void DestroyEntity(int entityId);
		void DestroyEntity(const std::vector<int>& entityIds);
		void DestroyEntity(Entity& entity);

		Entity& GetEntity(int entityId) { return m_Entities.at(entityId); }
		ComponentRegistry* GetComponentRegistry() const { return m_ComponentRegistry.get(); }

		template <typename... Components>
		EntityQueryResult<Components...> QueryEntities();

		template<typename ComponentType>
		void AddComponent(Entity& entity);

		void DestroyAllEntities();
	private:

		template <typename... ComponentTypes>
		ArchetypeIdentifierMask GetArchetypeIds();
		template <typename ComponentType>
		void UpdateArchetypeId(Entity& entity, bool isAdd = true);

		EntityChunk* GetFirstAvailableChunk(ArchetypeIdentifierMask& id, int capacity);
		EntityChunk* CreateNewChunk(ArchetypeIdentifierMask& id, int capacity);

		template <typename T>
		std::type_index ReturnComponentInfo();

		std::unordered_map<ArchetypeIdentifierMask, std::vector<std::unique_ptr<EntityChunk>>, ArchetypeIdentifierMaskHash> m_EntityChunks;
		std::vector<Entity> m_Entities;
		std::vector<int> m_FreeIndices;
		std::unique_ptr<ComponentRegistry> m_ComponentRegistry;
		int m_CurrentEntityId = 0;

		friend class Singleton<EntityManager>;
		EntityManager() = default;
	};

	template<typename ...Components>
	inline Entity& EntityManager::CreateEntity(int newChunkCapacity)
	{
		// static_assert((std::is_base_of_v<Component, Components> && ...), "All types must be derived from Component");
		//std::cout << "---------- Creating Entity ----------" << std::endl;

		ArchetypeIdentifierMask identifier = GetArchetypeIds<Components...>();
		EntityChunk* chunk = GetFirstAvailableChunk(identifier, newChunkCapacity);

		int id;

		if (!m_FreeIndices.empty())
		{
			id = m_FreeIndices.back();
			m_FreeIndices.pop_back();
			m_Entities[id] = Entity(id);
		}
		else
		{
			id = static_cast<int>(m_Entities.size());
			m_Entities.emplace_back(id);
		}

		m_Entities[id].SetCurrentChunk(identifier, static_cast<int>(m_EntityChunks[identifier].size()) - 1);
		chunk->AddEntity(id);

		//std::cout << "-------------------------------------" << std::endl;
		return m_Entities[id];
	}

	template<typename ComponentType>
	inline void EntityManager::AddComponent(Entity& entity)
	{
		auto* oldChunk = m_EntityChunks[entity.CurrentArchetypeId][entity.CurrentChunkIndex].get();
		UpdateArchetypeId<ComponentType>(entity);
	}

	template<typename ...Components>
	inline EntityQueryResult<Components...> EntityManager::QueryEntities()
	{
		EntityQueryResult<Components...> result;

		for (const auto& pair : m_EntityChunks)
		{
			if ((pair.first.HasComponent(Component<Components>::Index) && ...))
			{
				for (const std::unique_ptr<EntityChunk>& chunk : pair.second)
				{
					ChunkView<Components...> chunkView;
					chunkView.Entities = chunk->GetEntityIndices();
					chunkView.ComponentArrays = std::make_tuple(std::span<Components>(static_cast<Components*>(chunk->GetComponentArray(Component<Components>::Index)), chunk->GetEntityIndices().size())...);
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
	inline ArchetypeIdentifierMask EntityManager::GetArchetypeIds()
	{
		((m_ComponentRegistry->TryRegisterComponent<ComponentTypes>()), ...);
		ArchetypeIdentifierMask identifierMask;
		((identifierMask.AddComponent(Component<ComponentTypes>::Index)), ...);
		return identifierMask;
	}

	template<typename ComponentType>
	inline void EntityManager::UpdateArchetypeId(Entity& entity, bool isAdd)
	{
		if (isAdd)
		{
			ArchetypeIdentifierMask& archetypeId = entity.CurrentArchetypeId;
			archetypeId.AddComponent(Component<ComponentType>::Index);
		}
		else
		{
			ArchetypeIdentifierMask& archetypeId = entity.CurrentArchetypeId;
			// TODO: Remove from mask here
		}
	}
}