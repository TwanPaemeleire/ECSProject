#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Singleton.h"
#include <functional>
#include "Scene.h"
#include "Event.h"

namespace Bloodforge
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager() = default;

		void RequestSetCurrentScene(const std::string& name);
		void RegisterScene(const std::string& sceneName, std::function<void()> loadFunction);
		Scene& GetActiveScene() const { return *m_CurrentActiveScene; }

		Event<Scene&, Scene&>& GetOnActiveSceneSwitchedEvent() { return *m_OnActiveSceneSwitchedEvent; }

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderUI();

	private:
		void SetCurrentScene();

		std::unordered_map<std::string, std::unique_ptr<Scene>> m_SceneList;
		std::unique_ptr<Event<Scene&, Scene&>> m_OnActiveSceneSwitchedEvent;
		Scene* m_CurrentActiveScene = nullptr;
		Scene* m_SceneToSet = nullptr;
		bool m_ShouldSetScene = false;
	};
}