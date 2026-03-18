#pragma once
#include <memory>
#include <vector>
#include <functional>

namespace Bloodforge
{
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

		void RegisterSystem(std::unique_ptr<System> systemToRegister);
	private:
		std::vector<std::unique_ptr<System>> m_RegisteredSystems;
		std::function<void()> m_LoadFunction;
	};
}