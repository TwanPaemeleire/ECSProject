#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include <numeric>
#include <SpeedTracker.h>
#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <Bloodforge.h>
#include <SceneManager.h>
#include <SpriteSystem.h>
#include <Scene.h>
#include <SpriteSystem.h>
#include <memory>
#include <Texture2D.h>
#include <ResourceManager.h>
#include <filesystem>
#include <string>


void LoadFunction()
{
	auto& scene = Bloodforge::SceneManager::GetInstance().GetActiveScene();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();

	Bloodforge::Texture2D* texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Resources/Hades.png");		
	for (int i = 0; i < 1000; ++i)
	{
		Bloodforge::Entity& entity = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent>();
		Bloodforge::SpriteComponent* spriteComp = entityManager.GetComponent<Bloodforge::SpriteComponent>(entity);
		spriteComp->Texture = texture;
	}

	std::unique_ptr<Bloodforge::SpriteSystem> spriteSystem = std::make_unique<Bloodforge::SpriteSystem>();
	scene.RegisterSystem(std::move(spriteSystem));
}

int main(int, char* []) 
{
	// constexpr int MAX_STEPS_UP{ 5 };
	// std::filesystem::path resourcesFolderName{ "Resources" };
	// int counter{ 0 };
	// while (not std::filesystem::exists(dataFolderName) and counter < MAX_STEPS_UP)
	// {
	// 	std::filesystem::current_path(".."); // If "Resources" doesn't exist in this current folder, navigate one up in the hierarchy
	// 	counter++;
	// }
	// std::filesystem::current_path(resourcesFolderName); // Set the current path, this way the file system paths will be relative to this one and thus to "Resources" (if it found a "Resources" folder)

	Bloodforge::Bloodforge& engine = Bloodforge::Bloodforge::GetInstance();
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