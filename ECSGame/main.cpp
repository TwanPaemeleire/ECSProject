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
	for (int testIdx = 0; testIdx < 50; ++testIdx)
	{
		std::unique_ptr<SpeedTracker> speedTracker = std::make_unique<SpeedTracker>();
		speedTracker->StartTracking();

		std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();
		for (int i = 0; i < 1000; ++i)
		{
			Entity* entity = entityManager->CreateEntity<TransformComponent, SpriteComponent>();
			entityManager->DestroyEntity(entity);
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