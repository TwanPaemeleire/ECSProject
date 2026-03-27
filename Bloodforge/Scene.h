#pragma once
#include <memory>
#include <vector>
#include <functional>

namespace Bloodforge
{
	class System;

	template <typename SystemType>
	concept RegisterableSystem = std::is_base_of<System, SystemType>::value;

	class EntityManager;
	class System;

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
		std::function<void()> m_LoadFunction;
	};

	template<RegisterableSystem SystemType>
	inline void Scene::RegisterSystem()
	{
		m_RegisteredSystems.emplace_back(std::make_unique<SystemType>());
	}
}