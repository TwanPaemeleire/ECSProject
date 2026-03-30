#include "pch.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include <stdexcept>
#include "Scene.h"

namespace Bloodforge
{
	SceneManager::SceneManager()
	{
		m_OnActiveSceneSwitchedEvent = std::make_unique<Event<Scene&, Scene&>>();
	}

	void SceneManager::RequestSetCurrentScene(const std::string& name)
	{
		if (!m_SceneList.contains(name))
		{
			throw std::runtime_error("Scene with this name is not in the scene list");
		}
		m_SceneToSet = m_SceneList.at(name).get();
		m_ShouldSetScene = true;
		if (m_CurrentActiveScene == nullptr)
		{
			SetCurrentScene();
		}
	}

	void SceneManager::RegisterScene(const std::string& sceneName, std::function<void()> loadFunction)
	{
		if (m_SceneList.contains(sceneName))
		{
			throw std::runtime_error("Scene with this name is already registered");
		}
		else
		{
			m_SceneList.emplace(sceneName, std::make_unique<Scene>(loadFunction));
		}
	}

	void SceneManager::Start()
	{
		m_CurrentActiveScene->Start();
	}

	void SceneManager::Update()
	{
		if (m_ShouldSetScene)
		{
			SetCurrentScene();
		}
		m_CurrentActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		m_CurrentActiveScene->FixedUpdate();
	}

	void SceneManager::LateUpdate()
	{
		m_CurrentActiveScene->LateUpdate();
	}

	void SceneManager::Render() const
	{
		m_CurrentActiveScene->Render();
	}

	void SceneManager::RenderUI()
	{
		m_CurrentActiveScene->RenderUI();
	}

	void SceneManager::SetCurrentScene()
	{
		Scene* newScene = m_SceneToSet;
		Scene* prevScene = m_CurrentActiveScene;
		m_CurrentActiveScene = newScene;
		m_CurrentActiveScene->Load();
		if (prevScene != nullptr)
		{
			prevScene->RemoveAndResetAll();
		}
		m_OnActiveSceneSwitchedEvent->Invoke(*prevScene, *newScene);
		m_CurrentActiveScene->Start();
		m_ShouldSetScene = false;
		return;
	}
}