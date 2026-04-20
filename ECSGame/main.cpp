#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <Bloodforge.h>
#include <SceneManager.h>
#include <Scene.h>
#include <ResourceManager.h>
#include "TestSystem.h"
#include "RotationComponent.h"
#include <TextComponent.h>
#include <FileSaveLoadUtils.h>
#include "TestSaveFile.h"
#include <WindowUtils.h>
#include <IdCreator.h>
#include <RectColliderComponent.h>
#include <BloodRenderer.h>
#include <SpriteAnimatorComponent.h>
#include <AudioSourceComponent.h>
#include <FunctionInvokeComponent.h>
#include <AudioSourceSystem.h>

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
	}
	//////////

	scene.RegisterSystem<TestSystem>();
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
	Bloodforge::WindowUtils::SetWindowIcon("Heart.png");
	Bloodforge::WindowUtils::SetCustomCursor(Bloodforge::ResourceManager::GetInstance().LoadCustomCursor(CreateId("TestCursor"), "Cursor.png", 0, 0));

	auto& sceneManager = Bloodforge::SceneManager::GetInstance();
	sceneManager.RegisterScene("TestScene", LoadFunction);
	sceneManager.RequestSetCurrentScene("TestScene");
	engine.Run();
	return 0;
}