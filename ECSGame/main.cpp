#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <EntityManager.h>
#include <Bloodforge.h>
#include <ResourceManager.h>
#include <FileSaveLoadUtils.h>
#include "TestSaveFile.h"
#include <WindowUtils.h>
#include <IdCreator.h>
#include <BloodRenderer.h>
#include <SceneSystemManager.h>
#include <InputHandler.h>

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
#include <SceneDataComponent.h>
#include <SceneManagingDataComponent.h>
#include "SceneSwitchDataHolder.h"
#include "EnemySpawnData.h"

void InitializeRectColliderComponent(Bloodforge::Entity& entity, const Bloodforge::Vector2& size, const Bloodforge::Vector2& offset = { 0.0f, 0.0f })
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::RectColliderComponent* collider = entityManager.GetComponent<Bloodforge::RectColliderComponent>(entity);
	collider->SetSize(size);
	collider->SetOffset(offset);
}

void LoadFunction()
{
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

	//////////
	Bloodforge::Entity& spawnDataEntity = entityManager.CreateEntity();
	entityManager.AddComponent<EnemySpawnData>(spawnDataEntity);
	//////////

	Bloodforge::SceneSystemManager::GetInstance().TryRegisterSystem<EnemySpawnSystem>();
	Bloodforge::SceneSystemManager::GetInstance().TryRegisterSystem<PlayerTowerSystem>();
	Bloodforge::SceneSystemManager::GetInstance().TryRegisterSystem<ProjectileSystem>();
	Bloodforge::SceneSystemManager::GetInstance().TryRegisterSystem<InputTesterSystem>();
}

void SecondLoadFunction()
{
	auto& renderer = Bloodforge::BloodRenderer::GetInstance();
	renderer.SetBackgroundColor({ 127, 127, 127, 0 });
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

	auto& entityManager = Bloodforge::EntityManager::GetInstance();

	Bloodforge::Entity& sceneManagingEntity = Bloodforge::EntityManager::GetInstance().CreateEntity();
	sceneManagingEntity.DontDestroyOnSceneSwitch = true;
	Bloodforge::SceneManagingDataComponent* sceneManagingData = entityManager.AddComponent<Bloodforge::SceneManagingDataComponent>(sceneManagingEntity);

	Bloodforge::Entity& sceneDataEntity = Bloodforge::EntityManager::GetInstance().CreateEntity();
	int sceneDataEntityId = sceneDataEntity.Id;
	sceneDataEntity.DontDestroyOnSceneSwitch = true;
	Bloodforge::SceneDataComponent* sceneData = entityManager.AddComponent<Bloodforge::SceneDataComponent>(sceneDataEntity);
	sceneData->LoadFunction = LoadFunction;
	sceneData->SceneName = "TestScene";

	Bloodforge::Entity& sceneDataEntity2 = Bloodforge::EntityManager::GetInstance().CreateEntity();
	int sceneDataEntity2Id = sceneDataEntity2.Id;
	sceneDataEntity2.DontDestroyOnSceneSwitch = true;
	Bloodforge::SceneDataComponent* sceneData2 = entityManager.AddComponent<Bloodforge::SceneDataComponent>(sceneDataEntity2);
	sceneData2->LoadFunction = SecondLoadFunction;
	sceneData2->SceneName = "TestScene2";
	sceneManagingData->ShouldLoadScene = true;
	sceneManagingData->SceneToLoadDataEntityId = sceneDataEntity2.Id;

	Bloodforge::Entity& sceneSwitchDataHolderEntity = Bloodforge::EntityManager::GetInstance().CreateEntity();
	sceneSwitchDataHolderEntity.DontDestroyOnSceneSwitch = true;
	SceneSwitchDataHolder* sceneSwitchDataHolder = entityManager.AddComponent<SceneSwitchDataHolder>(sceneSwitchDataHolderEntity);
	sceneSwitchDataHolder->NextSceneId = sceneDataEntityId;
	sceneSwitchDataHolder->PreviousSceneId = sceneDataEntity2Id;

	Bloodforge::InputHandler& inputHandler = Bloodforge::InputHandler::GetInstance();
	inputHandler.CreateMap(CreateId("TestMap"));
	inputHandler.CreateAction(CreateId("SwitchScene"), CreateId("TestMap"),BLOODFORGE_KEYCODE_SPACE);
	inputHandler.AddListenerToInputAction(CreateId("SwitchScene"), CreateId("TestMap"), [&sceneManagingData, &sceneSwitchDataHolder](const Bloodforge::InputActionInfo& info)
		{
			if (!info.finished) return;
			sceneManagingData->ShouldLoadScene = true;
			sceneManagingData->SceneToLoadDataEntityId = sceneSwitchDataHolder->NextSceneId;
			std::swap(sceneSwitchDataHolder->PreviousSceneId, sceneSwitchDataHolder->NextSceneId);
		});
	inputHandler.SetCurrentMap(CreateId("TestMap"));

	inputHandler.CreateAction(CreateId("TestAction"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_LEFT);
	inputHandler.CreateAction(CreateId("TestActionMotion"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_MOTION);

	engine.Run();
	return 0;
}