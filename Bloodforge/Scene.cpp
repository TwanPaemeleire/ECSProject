#include "pch.h"
#include "Scene.h"
#include "EntityManager.h"
#include "System.h"

#include "SpriteSystem.h"
#include "TextSystem.h"
#include "CollisionSystem.h"

namespace Bloodforge
{
	Scene::Scene(std::function<void()> loadFunction)
		:m_LoadFunction{loadFunction}
	{
	}

	Scene::~Scene() = default;

	void Scene::Load()
	{
		RegisterSystem<SpriteSystem>();
		RegisterSystem<TextSystem>();
		RegisterSystem<CollisionSystem>();

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
}