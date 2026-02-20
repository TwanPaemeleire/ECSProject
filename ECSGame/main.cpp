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

int main(int, char* []) 
{
	std::unique_ptr<SpeedTracker> speedTracker = std::make_unique<SpeedTracker>();
	speedTracker->StartTracking();

	std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();
	entityManager->CreateEntity<TransformComponent, SpriteComponent>();
	entityManager->CreateEntity<SpriteComponent, TransformComponent>();
	entityManager->CreateEntity<TransformComponent, SpriteComponent>();
	entityManager->CreateEntity<TransformComponent, SpriteComponent>();
	entityManager->CreateEntity<TransformComponent, SpriteComponent>();

	int timeInMs = speedTracker->StopTracking();
	std::cout << "Time taken: " << timeInMs << " micro seconds" << std::endl;
	return 0;
}