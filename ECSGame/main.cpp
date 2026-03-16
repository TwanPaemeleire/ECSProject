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

int main(int, char* []) 
{
	std::vector<float> timings = std::vector<float>();
	Bloodforge::EntityManager& entityManager = Bloodforge::EntityManager::Instance;
	std::unique_ptr<Bloodforge::SpeedTracker> speedTracker = std::make_unique<Bloodforge::SpeedTracker>();
	for (int testIdx = 0; testIdx < 20; ++testIdx)
	{
		entityManager.DestroyAllEntities();
		for (int i = 0; i < 1000; ++i)
		{
			Bloodforge::Entity& entity = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent>();
		}

		speedTracker->StartTracking();
		Bloodforge::EntityQueryResult result = entityManager.QueryEntities<Bloodforge::TransformComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			//std::cout << "CHUNK" << std::endl;
			for (auto entity : view.Entities)
			{
				//std::cout << "Entity ID: " << entity << std::endl;
			}
		
			for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
			{
				std::get<0>(view.ComponentArrays)[i].TestVar = i;
			}
		}
		float timeInMs = speedTracker->StopTracking();

		std::cout << "Time taken: " << std::to_string(timeInMs) << " miliseconds" << std::endl;
		timings.push_back(timeInMs);
	}

	std::cout << "Average time taken: " << std::accumulate(timings.begin(), timings.end(), 0.0f) / timings.size() << " miliseconds" << std::endl;
	std::cout << "Longest time taken: " << *std::max_element(timings.begin(), timings.end()) << " miliseconds" << std::endl;
	std::cout << "Shortest time taken: " << *std::min_element(timings.begin(), timings.end()) << " miliseconds" << std::endl;

	std::cout << "ID of TransformComponent: " << Bloodforge::Component<Bloodforge::TransformComponent>::Index << std::endl;
	std::cout << "ID of SpriteComponent: " << Bloodforge::Component<Bloodforge::SpriteComponent>::Index << std::endl;
	return 0;
}