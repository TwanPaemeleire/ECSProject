#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <Bloodforge.h>
#include <SceneManager.h>
#include <Scene.h>
#include <memory>
#include <Texture2D.h>
#include <ResourceManager.h>
#include "TestSystem.h"
#include "RotationComponent.h"
#include <TextComponent.h>
#include <FileSaveLoadUtils.h>
#include "TestSaveFile.h"
#include <WindowUtils.h>
#include <SdbmHash.h>

void LoadFunction()
{
	auto& scene = Bloodforge::SceneManager::GetInstance().GetActiveScene();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();

	Bloodforge::Entity& entity = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, RotationComponent>();
	Bloodforge::SpriteComponent* spriteComp = entityManager.GetComponent<Bloodforge::SpriteComponent>(entity);
	spriteComp->Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Heart.png");
	Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(entity);
	transformComp->SetLocalPosition(300.0f, 300.0f);
	RotationComponent* rotationComp = entityManager.GetComponent<RotationComponent>(entity);
	rotationComp->CenterPosition = { 400.0f, 400.0f };
	rotationComp->Speed = 5.0f;
	rotationComp->Radius = 200.0f;

	Bloodforge::Entity& entity3 = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::TextComponent>();
	Bloodforge::TextComponent* textComp = entityManager.GetComponent<Bloodforge::TextComponent>(entity3);
	textComp->SetText("Testing");
	textComp->SetFontSize(60.0f);
	textComp->SetColor(Bloodforge::Color(255, 0, 0, 255));
	textComp->SetFont(Bloodforge::ResourceManager::GetInstance().LoadFont("Font.otf", 60.0f));
	entityManager.GetComponent<Bloodforge::TransformComponent>(entity3)->SetLocalPosition(400, 400);

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

	Bloodforge::WindowUtils::SetWindowAlwaysOnTop(true);
	Bloodforge::WindowUtils::SetWindowBordered(true);
	Bloodforge::WindowUtils::SetWindowFullScreen(false);
	Bloodforge::WindowUtils::SetWindowIcon("Heart.png");
	Bloodforge::WindowUtils::SetCustomCursor(Bloodforge::ResourceManager::GetInstance().LoadCustomCursor(make_sdbm_hash("TestCursor"), "Cursor.png", 0, 0));

	auto& sceneManager = Bloodforge::SceneManager::GetInstance();
	sceneManager.RegisterScene("TestScene", LoadFunction);
	sceneManager.RequestSetCurrentScene("TestScene");
	engine.Run();

	// std::vector<float> timings = std::vector<float>();
	// Bloodforge::EntityManager& entityManager = Bloodforge::EntityManager::GetInstance();
	// std::unique_ptr<Bloodforge::SpeedTracker> speedTracker = std::make_unique<Bloodforge::SpeedTracker>();
	// for (int testIdx = 0; testIdx < 20; ++testIdx)
	// {
	// 	entityManager.DestroyAllEntities();
	// 	for (int i = 0; i < 1000; ++i)
	// 	{
	// 		Bloodforge::Entity& entity = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent>();
	// 	}
	// 
	// 	speedTracker->StartTracking();
	// 	Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::TransformComponent>();
	// 	for (Bloodforge::ChunkView view : result.Chunks)
	// 	{
	// 		//std::cout << "CHUNK" << std::endl;
	// 		for (auto entity : view.Entities)
	// 		{
	// 			//std::cout << "Entity ID: " << entity << std::endl;
	// 		}
	// 	
	// 		for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
	// 		{
	// 			std::get<0>(view.ComponentArrays)[i].TestVar = i;
	// 		}
	// 	}
	// 	float timeInMs = speedTracker->StopTracking();
	// 
	// 	std::cout << "Time taken: " << std::to_string(timeInMs) << " miliseconds" << std::endl;
	// 	timings.push_back(timeInMs);
	// }
	// 
	// std::cout << "Average time taken: " << std::accumulate(timings.begin(), timings.end(), 0.0f) / timings.size() << " miliseconds" << std::endl;
	// std::cout << "Longest time taken: " << *std::max_element(timings.begin(), timings.end()) << " miliseconds" << std::endl;
	// std::cout << "Shortest time taken: " << *std::min_element(timings.begin(), timings.end()) << " miliseconds" << std::endl;
	// 
	// std::cout << "ID of TransformComponent: " << Bloodforge::Component<Bloodforge::TransformComponent>::Index << std::endl;
	// std::cout << "ID of SpriteComponent: " << Bloodforge::Component<Bloodforge::SpriteComponent>::Index << std::endl;
	return 0;
}