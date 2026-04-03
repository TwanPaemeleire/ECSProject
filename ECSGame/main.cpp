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
#include <SdbmHash.h>
#include <RectColliderComponent.h>
#include <BloodRenderer.h>

void LoadFunction()
{
	auto& scene = Bloodforge::SceneManager::GetInstance().GetActiveScene();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();

	Bloodforge::Entity& entity = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();
	int entityId = entity.Id;
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity)->Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Heart.png");
	Bloodforge::TransformComponent* transformComp1 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity);
	transformComp1->SetLocalPosition(0.0f, 0.0f);
	// transformComp1->SetLocalScale({ 2.0f, 2.0f });
	RotationComponent* rotComp = entityManager.GetComponent<RotationComponent>(entity);
	rotComp->SpeedDegPerSec = 25.0f;

	Bloodforge::Entity& entity2 = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();
	int entity2Id = entity2.Id;
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity2)->Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Heart.png");
	Bloodforge::TransformComponent* transformComp2 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity2);
	transformComp2->SetParent(entityId);
	transformComp2->SetLocalPosition(100.0f, 100.0f);
	transformComp2->SetLocalScale({2.0f, 1.0f});
	transformComp2->SetLocalRotation(10.0f);
	RotationComponent* rotComp2 = entityManager.GetComponent<RotationComponent>(entity2);
	rotComp2->SpeedDegPerSec = 90.f;

	Bloodforge::Entity& entity3 = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity3)->Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Heart.png");
	Bloodforge::TransformComponent* transformComp3 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity3);
	transformComp3->SetParent(entity2Id);
	transformComp3->SetLocalPosition(50.0f, 50.0f);
	transformComp3->SetLocalRotation(45.0f);
	transformComp3->SetLocalScale({ 0.5f, 0.5f });
	RotationComponent* rotComp3 = entityManager.GetComponent<RotationComponent>(entity3);
	rotComp3->SpeedDegPerSec = 180.f;


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
	Bloodforge::WindowUtils::SetCustomCursor(Bloodforge::ResourceManager::GetInstance().LoadCustomCursor(make_sdbm_hash("TestCursor"), "Cursor.png", 0, 0));

	auto& sceneManager = Bloodforge::SceneManager::GetInstance();
	sceneManager.RegisterScene("TestScene", LoadFunction);
	sceneManager.RequestSetCurrentScene("TestScene");
	engine.Run();
	return 0;
}