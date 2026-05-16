#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <EntityManager.h>
#include <Bloodforge.h>
#include <SceneManager.h>
#include <ResourceManager.h>
#include <FileSaveLoadUtils.h>
#include "TestSaveFile.h"
#include <WindowUtils.h>
#include <IdCreator.h>
#include <BloodRenderer.h>

#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <RectColliderComponent.h>
#include <SpriteAnimatorComponent.h>
#include "EnemySpawnSystem.h"
#include "PlayerTowerSystem.h"
#include "ProjectileSystem.h"
#include "InputTesterSystem.h"
#include "PlayerTower.h"
#include "Health.h"

void InitializeRectColliderComponent(Bloodforge::Entity& entity, const Bloodforge::Vector2& size, const Bloodforge::Vector2& offset = { 0.0f, 0.0f })
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::RectColliderComponent* collider = entityManager.GetComponent<Bloodforge::RectColliderComponent>(entity);
	collider->SetSize(size);
	collider->SetOffset(offset);
}

void LoadFunction()
{
	auto& scene = Bloodforge::SceneManager::GetInstance().GetActiveScene();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	auto& renderer = Bloodforge::BloodRenderer::GetInstance();
	renderer.SetBackgroundColor({ 127, 127, 127, 0 });

	//////////
	Bloodforge::Entity& towerEntity = entityManager.CreateEntity();
	towerEntity.Name = "PlayerTower";
	towerEntity.Tag = CreateId("PlayerTower");
	int towerEntityId = towerEntity.Id;

	{
		entityManager.AddComponent<Bloodforge::SpriteComponent>(towerEntityId);
		Bloodforge::SpriteAnimatorComponent* animComp = entityManager.AddComponent<Bloodforge::SpriteAnimatorComponent>(towerEntityId);
		Bloodforge::AnimationData towerIdleData;
		towerIdleData.FrameTime = 0.05f;
		towerIdleData.ShouldLoop = true;
		towerIdleData.NumberOfFrames = 8;
		towerIdleData.Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Tower/TowerIdle.png");
		animComp->AddAnimation(CreateId("TowerIdle"), towerIdleData);
		animComp->PlayAnimation(CreateId("TowerIdle"));
		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(towerEntityId);
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 2.0f);
		entityManager.AddComponent<PlayerTower>(towerEntityId);
		entityManager.AddComponent<Health>(towerEntityId);
		Bloodforge::RectColliderComponent* rectCollider = entityManager.AddComponent<Bloodforge::RectColliderComponent>(towerEntityId);
		rectCollider->SetSize({ 350.0f, 350.0f });
	}
	//////////

	scene.RegisterSystem<EnemySpawnSystem>();
	scene.RegisterSystem<PlayerTowerSystem>();
	scene.RegisterSystem<ProjectileSystem>();
	scene.RegisterSystem<InputTesterSystem>();
}

int main(int, char* []) 
{
	Bloodforge::Bloodforge& engine = Bloodforge::Bloodforge::GetInstance();
	engine.SetResourcesDirectory("Resources");

	TestSaveFile testSaveFile;
	testSaveFile.Coins = 200;
	testSaveFile.Health = 50.0f;
	Bloodforge::FileSaveLoadUtils::SaveFile<TestSaveFile>(testSaveFile, "TestSaveFile.json");
	testSaveFile = Bloodforge::FileSaveLoadUtils::LoadFile<TestSaveFile>("TestSaveFile.json");

	Bloodforge::WindowUtils::SetWindowAlwaysOnTop(false);
	Bloodforge::WindowUtils::SetWindowBordered(true);
	Bloodforge::WindowUtils::SetWindowFullScreen(false);
	Bloodforge::WindowUtils::SetWindowSize(950, 950);
	Bloodforge::WindowUtils::SetWindowIcon("Heart.png");
	Bloodforge::WindowUtils::SetCustomCursor(Bloodforge::ResourceManager::GetInstance().LoadCustomCursor(CreateId("TestCursor"), "Cursor.png", 0, 0));

	auto& sceneManager = Bloodforge::SceneManager::GetInstance();
	sceneManager.RegisterScene("TestScene", LoadFunction);
	sceneManager.RequestSetCurrentScene("TestScene");
	engine.Run();
	return 0;
}