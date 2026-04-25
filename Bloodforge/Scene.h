#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <typeindex>
#include <stdexcept>
#include <algorithm>
#include <iterator>

namespace Bloodforge
{
	class System;
	class EntityManager;

	template <typename SystemType>
	concept RegisterableSystem = std::is_base_of<System, SystemType>::value;

	class Scene
	{
	public:
		Scene(std::function<void()> loadFunction);
		~Scene();

		void Load();
		void RemoveAndResetAll();

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderUI();

		template <RegisterableSystem SystemType>
		void RegisterSystem();

		template <RegisterableSystem SystemType>
		SystemType* GetSystem();

	private:
		std::vector<std::unique_ptr<System>> m_RegisteredSystems;
		std::unordered_map<std::type_index, int> m_SystemMap;

		std::function<void()> m_LoadFunction;
	};

	template<RegisterableSystem SystemType>
	inline void Scene::RegisterSystem()
	{
		std::type_index typeIndex = std::type_index(typeid(SystemType));
		if (!m_SystemMap.contains(typeIndex))
		{
			m_RegisteredSystems.emplace_back(std::make_unique<SystemType>());
			m_SystemMap.insert({ typeIndex, static_cast<int>(m_RegisteredSystems.size()) - 1 });
		}
		else
		{
			throw std::runtime_error("Trying to registered a system that has already been registered.");
		}
	}

	template<RegisterableSystem SystemType>
	inline SystemType* Scene::GetSystem()
	{
		std::type_index typeIndex = std::type_index(typeid(SystemType));
		if (m_SystemMap.contains(typeIndex))
		{
			return static_cast<SystemType*>(m_RegisteredSystems[m_SystemMap[typeIndex]].get());
		}
		m_RegisteredSystems.emplace_back(std::make_unique<SystemType>());
		m_SystemMap.insert({ typeIndex, static_cast<int>(m_RegisteredSystems.size()) - 1 });
		return static_cast<SystemType*>(m_RegisteredSystems[m_SystemMap[typeIndex]].get());
	}
}