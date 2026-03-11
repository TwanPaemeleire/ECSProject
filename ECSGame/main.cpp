#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "SpeedTracker.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <iostream>
#include <numeric>

int main(int, char* []) 
{
	std::vector<float> timings = std::vector<float>();
	for (int testIdx = 0; testIdx < 20; ++testIdx)
	{

		std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();
		for (int i = 0; i < 100000; ++i)
		{
			Entity* entity = entityManager->CreateEntity<SpriteComponent>();
		}
		for (int i = 0; i < 10; ++i)
		{
			Entity* entity = entityManager->CreateEntity<TransformComponent, SpriteComponent>();
		}

		std::unique_ptr<SpeedTracker> speedTracker = std::make_unique<SpeedTracker>();
		speedTracker->StartTracking();

		EntityQueryResult result = entityManager->QueryEntities<TransformComponent>();
		for (ChunkView view : result.Chunks)
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
		std::cout << "Time taken: " << timeInMs << " miliseconds" << std::endl;
		timings.push_back(timeInMs);
	}

	std::cout << "Average time taken: " << std::accumulate(timings.begin(), timings.end(), 0.0f) / timings.size() << " miliseconds" << std::endl;
	std::cout << "Longest time taken: " << *std::max_element(timings.begin(), timings.end()) << " miliseconds" << std::endl;
	std::cout << "Shortest time taken: " << *std::min_element(timings.begin(), timings.end()) << " miliseconds" << std::endl;

	std::cout << "ID of TransformComponent: " << Component<TransformComponent>::Index << std::endl;
	std::cout << "ID of SpriteComponent: " << Component<SpriteComponent>::Index << std::endl;
	return 0;
}