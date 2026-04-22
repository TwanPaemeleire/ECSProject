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
#include "TransformComponent.h"
#include "Singleton.h"

using TagId = unsigned int;

namespace Bloodforge
{
	struct Entity;

	template <typename... Components>
	struct ChunkView
	{
		std::span<int> Entities;
		std::tuple<std::span<Components>...> ComponentArrays;

		template <typename ComponentType>
		std::span<ComponentType> GetComponentArray()
		{
			return std::get<std::span<ComponentType>>(ComponentArrays);
		}
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

		Entity& CreateEntity(int newChunkCapacity = 20);

		void DestroyEntity(int entityId);
		void DestroyEntity(const std::vector<int>& entityIds);
		void DestroyEntity(Entity& entity);

		Entity& GetEntity(int entityId) { return m_Entities.at(entityId); }
		ComponentRegistry* GetComponentRegistry() const { return m_ComponentRegistry.get(); }

		template <typename... Components>
		EntityQueryResult<Components...> QueryEntities();

		template<typename ComponentType>
		ComponentType* AddComponent(Entity& entity);
		template<typename ComponentType>
		ComponentType* AddComponent(int entityId);
		template<typename ComponentType>
		void RemoveComponent(Entity& entity);
		template<typename ComponentType>
		void RemoveComponent(int entityId);
		template <typename ComponentType>
		ComponentType* GetComponent(Entity& entity);
		template <typename ComponentType>
		ComponentType* GetComponent(int entityId);
		template <typename ComponentType>
		ComponentType* GetComponentInChildren(Entity& entity);
		template <typename ComponentType>
		ComponentType* GetComponentInChildren(int entityId);
		template <typename ComponentType>
		ComponentType* GetComponentInParent(Entity& entity);
		template <typename ComponentType>
		ComponentType* GetComponentInParent(int entityId);
		template <typename ComponentType>
		bool HasComponent(int entityId);
		template <typename ComponentType>
		bool HasComponent(Entity& entity);

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
		EntityManager();
	};

	inline Entity& EntityManager::CreateEntity(int newChunkCapacity)
	{
		//std::cout << "---------- Creating Entity ----------" << std::endl;

		ArchetypeIdentifierMask identifier = GetArchetypeIds<TransformComponent>();
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
			m_Entities.emplace_back(Entity(id));
		}

		m_Entities[id].SetCurrentChunk(identifier, static_cast<int>(m_EntityChunks[identifier].size()) - 1);
		chunk->AddEntity(id);

		// Construct transform component
		m_ComponentRegistry->TryRegisterComponent<TransformComponent>();
		int compId = Component<TransformComponent>::Index;
		void* array = chunk->GetComponentArray(compId);
		auto& info = m_ComponentRegistry->GetComponentInfo(compId);

		void* elementPtr = static_cast<char*>(array) + chunk->GetEntityInChunkIndex(id) * info.Size;
		info.Construct(elementPtr, id);

		//std::cout << "-------------------------------------" << std::endl;
		return m_Entities[id];
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::AddComponent(Entity& entity)
	{
		if (entity.CurrentArchetypeId.HasComponent(Component<ComponentType>::Index))
		{
			throw std::exception("Trying to add a component to an entity that already has it.");
		}
		m_ComponentRegistry->TryRegisterComponent<ComponentType>();

		// Get old chunk and archetype
		auto* oldChunk = m_EntityChunks[entity.CurrentArchetypeId][entity.CurrentChunkIndex].get();
		ArchetypeIdentifierMask oldArchetypeMask = entity.CurrentArchetypeId;

		// Create new archetype and chunk
		ArchetypeIdentifierMask newArchetypeMask = oldArchetypeMask;
		newArchetypeMask.AddComponent(Component<ComponentType>::Index);
		EntityChunk* newChunk = GetFirstAvailableChunk(newArchetypeMask, 20);

		// Add entity to new chunk
		int newIndex = newChunk->AddEntity(entity.Id);

		// Copy old components
		for (int componentId : oldArchetypeMask.GetComponentIndices())
		{
			void* srcArray = oldChunk->GetComponentArray(componentId);
			void* dstArray = newChunk->GetComponentArray(componentId);
			int oldIndex = oldChunk->GetEntityInChunkIndex(entity.Id);
			auto& info = m_ComponentRegistry->GetComponentInfo(componentId);

			std::memcpy(static_cast<char*>(dstArray) + newIndex * info.Size, static_cast<char*>(srcArray) + oldIndex * info.Size, info.Size);
		}

		// Construct new component
		int compId = Component<ComponentType>::Index;
		void* array = newChunk->GetComponentArray(compId);
		auto& info = m_ComponentRegistry->GetComponentInfo(compId);

		void* elementPtr = static_cast<char*>(array) + newIndex * info.Size;
		info.Construct(elementPtr, entity.Id);

		// Remove from old chunk
		oldChunk->RemoveEntityAndComponents(entity);

		// Update entity data
		entity.CurrentArchetypeId = newArchetypeMask;
		entity.CurrentChunkIndex = newChunk->GetChunkIndex();

		return GetComponent<ComponentType>(entity);
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::AddComponent(int entityId)
	{
		return AddComponent<ComponentType>(GetEntity(entityId));
	}

	template<typename ComponentType>
	inline void EntityManager::RemoveComponent(Entity& entity)
	{
		if (!entity.CurrentArchetypeId.HasComponent(Component<ComponentType>::Index))
		{
			throw std::exception("Trying to remove a component from an entity that doesn't have it.");
		}
		m_ComponentRegistry->TryRegisterComponent<ComponentType>();

		// Get old chunk and archetype
		auto* oldChunk = m_EntityChunks[entity.CurrentArchetypeId][entity.CurrentChunkIndex].get();
		ArchetypeIdentifierMask oldArchetypeMask = entity.CurrentArchetypeId;

		// Create new archetype and chunk
		ArchetypeIdentifierMask newArchetypeMask = oldArchetypeMask;
		newArchetypeMask.RemoveComponent(Component<ComponentType>::Index);
		EntityChunk* newChunk = GetFirstAvailableChunk(newArchetypeMask, 20);

		// Add entity to new chunk
		int newIndex = newChunk->AddEntity(entity.Id);

		// Copy old components
		for (int componentId : newArchetypeMask.GetComponentIndices())
		{
			void* srcArray = oldChunk->GetComponentArray(componentId);
			void* dstArray = newChunk->GetComponentArray(componentId);
			int oldIndex = oldChunk->GetEntityInChunkIndex(entity.Id);
			auto& info = m_ComponentRegistry->GetComponentInfo(componentId);

			std::memcpy(static_cast<char*>(dstArray) + newIndex * info.Size, static_cast<char*>(srcArray) + oldIndex * info.Size, info.Size);
		}

		// Destruct old component
		int compId = Component<ComponentType>::Index;
		void* array = oldChunk->GetComponentArray(compId);
		auto& info = m_ComponentRegistry->GetComponentInfo(compId);

		int oldIndex = oldChunk->GetEntityInChunkIndex(entity.Id);
		void* elementPtr = static_cast<char*>(array) + oldIndex * info.Size;
		info.Destruct(elementPtr);

		// Remove from old chunk
		oldChunk->RemoveEntityAndComponents(entity);

		// Update entity data
		entity.CurrentArchetypeId = newArchetypeMask;
		entity.CurrentChunkIndex = newChunk->GetChunkIndex();
	}

	template<typename ComponentType>
	inline void EntityManager::RemoveComponent(int entityId)
	{
		RemoveComponent(GetEntity(entityId));
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::GetComponent(Entity& entity)
	{
		auto* chunk = m_EntityChunks[entity.CurrentArchetypeId][entity.CurrentChunkIndex].get();
		void* array = chunk->GetComponentArray(Component<ComponentType>::Index);
		if (!array) return nullptr;
		int index = chunk->GetEntityInChunkIndex(entity.Id);
		return &static_cast<ComponentType*>(array)[index];
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::GetComponent(int entityId)
	{
		return GetComponent<ComponentType>(GetEntity(entityId));
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::GetComponentInChildren(Entity& entity)
	{
		for (int childId : GetComponent<TransformComponent>(entity)->GetChildEntityIds())
		{
			ComponentType* component = GetComponent<ComponentType>(childId);
			if (component) return component;
			component = GetComponentInChildren<ComponentType>(childId);
			if (component) return component;
		}
		return nullptr;
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::GetComponentInChildren(int entityId)
	{
		return GetComponentInChildren<ComponentType>(GetEntity(entityId));
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::GetComponentInParent(Entity& entity)
	{
		TransformComponent* transform = GetComponent<TransformComponent>(entity);
		GetComponent<ComponentType>(transform->GetParentEntityId());
		return nullptr;
	}

	template<typename ComponentType>
	inline ComponentType* EntityManager::GetComponentInParent(int entityId)
	{
		return GetComponentInParent(GetEntity(entityId));
	}

	template<typename ComponentType>
	inline bool EntityManager::HasComponent(int entityId)
	{
		return GetEntity(entityId).CurrentArchetypeId.HasComponent(Component<ComponentType>::Index);
	}

	template<typename ComponentType>
	inline bool EntityManager::HasComponent(Entity& entity)
	{
		return HasComponent<ComponentType>(entity.Id);
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