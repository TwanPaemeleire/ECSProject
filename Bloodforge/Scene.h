#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <typeindex>
#include <stdexcept>
#include <algorithm>

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

	private:
		std::vector<std::unique_ptr<System>> m_RegisteredSystems;
		std::vector<std::type_index> m_RegisteredSystemTypes;
		std::function<void()> m_LoadFunction;
	};

	template<RegisterableSystem SystemType>
	inline void Scene::RegisterSystem()
	{
		std::type_index typeIndex = std::type_index(typeid(SystemType));
		auto it = std::find(m_RegisteredSystemTypes.begin(), m_RegisteredSystemTypes.end(), typeIndex);
		if (it == m_RegisteredSystemTypes.end())
		{
			m_RegisteredSystems.emplace_back(std::make_unique<SystemType>());
			m_RegisteredSystemTypes.emplace_back(typeIndex);
		}
		else
		{
			throw std::runtime_error("Trying to registered a system that has already been registered.");
		}
	}
}