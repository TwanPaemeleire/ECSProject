#include "pch.h"
#include "Scene.h"
#include "EntityManager.h"
#include "System.h"

namespace Bloodforge
{
	Scene::Scene(std::function<void()> loadFunction)
		:m_LoadFunction{loadFunction}
	{
	}

	Scene::~Scene() = default;

	void Scene::Load()
	{
		m_LoadFunction();
	}

	void Scene::RemoveAndResetAll()
	{
		EntityManager::GetInstance().DestroyAllEntities();
		m_RegisteredSystems.clear();
	}

	void Scene::Start()
	{
		for (const std::unique_ptr<System>& system : m_RegisteredSystems)
		{
			system->OnStart();
		}
	}

	void Scene::Update()
	{
		for (const std::unique_ptr<System>& system : m_RegisteredSystems)
		{
			system->OnUpdate();
		}
	}

	void Scene::FixedUpdate()
	{
		for (const std::unique_ptr<System>& system : m_RegisteredSystems)
		{
			system->OnFixedUpdate();
		}
	}

	void Scene::LateUpdate()
	{
		for (const std::unique_ptr<System>& system : m_RegisteredSystems)
		{
			system->OnLateUpdate();
		}
	}

	void Scene::Render() const
	{
		for (const std::unique_ptr<System>& system : m_RegisteredSystems)
		{
			system->OnRender();
		}
	}

	void Scene::RenderUI()
	{
		for (const std::unique_ptr<System>& system : m_RegisteredSystems)
		{
			system->OnRenderUI();
		}
	}

	void Bloodforge::Scene::RegisterSystem(std::unique_ptr<System> systemToRegister)
	{
		m_RegisteredSystems.emplace_back(std::move(systemToRegister));
	}
}