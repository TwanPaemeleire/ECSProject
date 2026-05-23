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
#include "EnemySpawnData.h"
#include <TextComponent.h>
#include "ButtonComponent.h"

void InitializeRectColliderComponent(Bloodforge::Entity& entity, const Bloodforge::Vector2& size, const Bloodforge::Vector2& offset = { 0.0f, 0.0f })
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::RectColliderComponent* collider = entityManager.GetComponent<Bloodforge::RectColliderComponent>(entity);
	collider->SetSize(size);
	collider->SetOffset(offset);
}

void GameLoadFunction()
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

void MainMenuLoadFunction()
{
	auto& renderer = Bloodforge::BloodRenderer::GetInstance();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	auto& resourceManager = Bloodforge::ResourceManager::GetInstance();
	Bloodforge::Font* font = resourceManager.LoadFont("Font.otf", 64.0f);

	// Background
	{
		Bloodforge::Entity& backgroundEntity = entityManager.CreateEntity();
		backgroundEntity.Name = "MainMenuBackground";
		Bloodforge::SpriteComponent* spriteComp = entityManager.AddComponent<Bloodforge::SpriteComponent>(backgroundEntity);
		spriteComp->DrawOrder = -1;
		Bloodforge::SpriteAnimatorComponent* spriteAnimComp = entityManager.AddComponent<Bloodforge::SpriteAnimatorComponent>(backgroundEntity);
		Bloodforge::AnimationData backgroundAnimData;
		backgroundAnimData.FrameTime = 0.1f;
		backgroundAnimData.ShouldLoop = true;
		backgroundAnimData.NumberOfFrames = 50;
		backgroundAnimData.Texture = resourceManager.LoadTexture("MainMenuBackground.png");
		spriteAnimComp->AddAnimation(CreateId("MainMenuBackgroundAnimation"), backgroundAnimData);
		spriteAnimComp->PlayAnimation(CreateId("MainMenuBackgroundAnimation"));
		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(backgroundEntity);
		transformComp->SetLocalScale({ 3.0f, 3.0f });
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 2.0f);
	}

	//Background behind
	{
		Bloodforge::Entity& backgroundBehindEntity = entityManager.CreateEntity();
		backgroundBehindEntity.Name = "MainMenuBackgroundBehind";
		Bloodforge::SpriteComponent* spriteComp = entityManager.AddComponent<Bloodforge::SpriteComponent>(backgroundBehindEntity);
		spriteComp->DrawOrder = -2;
		spriteComp->SetTexture(resourceManager.LoadTexture("MainMenuBackgroundBehind.png"));
		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(backgroundBehindEntity);
		transformComp->SetLocalScale({ 3.0f, 3.0f });
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 2.0f);
	}

	// Title
	{
		Bloodforge::Entity& titleEntity = entityManager.CreateEntity();
		titleEntity.Name = "MainMenuTitle";
		Bloodforge::TextComponent* textComp = entityManager.AddComponent<Bloodforge::TextComponent>(titleEntity);
		textComp->SetText("GameTitle");
		textComp->SetFont(font);
		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(titleEntity);
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 4.0f);
	}

	// Start button
	{
		Bloodforge::Entity& startButtonEntity = entityManager.CreateEntity();
		startButtonEntity.Name = "StartButton";
		Bloodforge::ButtonComponent* buttonComp = entityManager.AddComponent<Bloodforge::ButtonComponent>(startButtonEntity);
		buttonComp->NormalTexture = resourceManager.LoadTexture("StartButtonNormal.png");
		buttonComp->HoverTexture = resourceManager.LoadTexture("StartButtonHover.png");
		buttonComp->PressedTexture = resourceManager.LoadTexture("StartButtonPressed.png");
		buttonComp->NormalScale = { 1.0f, 1.0f };
		buttonComp->HoverScale = { 1.1f, 1.1f };
		buttonComp->OnClick.AddListener([](Bloodforge::ButtonComponent&)
			{
				auto& sceneManagingData = Bloodforge::EntityManager::GetInstance().GetFirstEntityWithComponents<Bloodforge::SceneManagingDataComponent>()->GetComponent<Bloodforge::SceneManagingDataComponent>();
				sceneManagingData.ShouldLoadScene = true;
				sceneManagingData.SceneToLoadDataEntityId = 2;
			});

		Bloodforge::SpriteComponent* spriteComp = entityManager.AddComponent<Bloodforge::SpriteComponent>(startButtonEntity);
		spriteComp->SetTexture(buttonComp->NormalTexture);
		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(startButtonEntity);
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 2.0f);
	}
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


	// Main menu scene data
	int mainMenuSceneDataEntityId = -1;
	{
		Bloodforge::Entity& mainMenuSceneDataEntity = Bloodforge::EntityManager::GetInstance().CreateEntity();
		mainMenuSceneDataEntityId = mainMenuSceneDataEntity.Id;
		mainMenuSceneDataEntity.DontDestroyOnSceneSwitch = true;
		Bloodforge::SceneDataComponent* mainMenuSceneData = entityManager.AddComponent<Bloodforge::SceneDataComponent>(mainMenuSceneDataEntity);
		mainMenuSceneData->LoadFunction = MainMenuLoadFunction;
		mainMenuSceneData->SceneName = "MainMenu";
	}

	// Game scene data
	{
		Bloodforge::Entity& sceneDataEntity = Bloodforge::EntityManager::GetInstance().CreateEntity();
		sceneDataEntity.DontDestroyOnSceneSwitch = true;
		Bloodforge::SceneDataComponent* sceneData = entityManager.AddComponent<Bloodforge::SceneDataComponent>(sceneDataEntity);
		sceneData->LoadFunction = GameLoadFunction;
		sceneData->SceneName = "GameScene";
	}


	sceneManagingData->ShouldLoadScene = true;
	sceneManagingData->SceneToLoadDataEntityId = mainMenuSceneDataEntityId;

	Bloodforge::InputHandler& inputHandler = Bloodforge::InputHandler::GetInstance();
	inputHandler.CreateMap(CreateId("TestMap"));
	inputHandler.CreateAction(CreateId("SwitchScene"), CreateId("TestMap"),BLOODFORGE_KEYCODE_SPACE);
	inputHandler.SetCurrentMap(CreateId("TestMap"));

	inputHandler.CreateAction(CreateId("TestAction"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_LEFT);
	inputHandler.CreateAction(CreateId("TestActionMotion"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_MOTION);

	engine.Run();
	return 0;
}