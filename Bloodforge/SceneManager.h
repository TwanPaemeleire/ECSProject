#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Singleton.h"
#include <functional>
#include "Scene.h"

namespace Bloodforge
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void RequestSetCurrentScene(const std::string& name);
		void RegisterScene(const std::string& sceneName, std::function<void()> loadFunction);
		void ChangeActiveScene(const std::string& sceneName);
		Scene& GetActiveScene() const { return *m_CurrentActiveScene; }

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderUI();

	private:
		void SetCurrentScene();

		std::unordered_map<std::string, std::unique_ptr<Scene>> m_SceneList;
		Scene* m_CurrentActiveScene = nullptr;
		Scene* m_SceneToSet = nullptr;
		bool m_ShouldSetScene = false;
	};
}