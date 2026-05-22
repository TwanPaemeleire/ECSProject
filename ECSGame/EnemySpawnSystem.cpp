#include "EnemySpawnSystem.h"
#include <EntityManager.h>
#include "Enemy.h"
#include <numbers>
#include <SpriteComponent.h>
#include <SpriteAnimatorComponent.h>
#include <ResourceManager.h>
#include <IdCreator.h>
#include <BloodRenderer.h>
#include <BloodTime.h>
#include <FunctionInvokeComponent.h>
#include <RectColliderComponent.h>
#include "Health.h"
#include "AchievementMonitor.h"
#include "EnemySpawnData.h"
using namespace Bloodforge;

void EnemySpawnSystem::OnStart()
{
	std::optional<EntityView<EnemySpawnData>> spawnDataEntity = EntityManager::GetInstance().GetFirstEntityWithComponents<EnemySpawnData>();
	if (spawnDataEntity.has_value())
	{
		SpawnEnemy();
		SpawnLoop();
	}
}

void EnemySpawnSystem::OnUpdate()
{
	EntityQueryResult<Enemy, TransformComponent> result = EntityManager::GetInstance().QueryEntities<Enemy, TransformComponent>();

	for (EntityView<Enemy, TransformComponent>& entityView : result.EntityViews)
	{
		Enemy& enemy = entityView.GetComponent<Enemy>();
		TransformComponent& transform = entityView.GetComponent<TransformComponent>();
		Vector2 directionToCenter = Vector2(BloodRenderer::GetInstance().GetWindowWidth() / 2.0f, BloodRenderer::GetInstance().GetWindowHeight() / 2.0f) - transform.GetLocalPosition();
		enemy.Direction = directionToCenter.Normalized();
		Vector2 movement = enemy.Direction * enemy.Speed * BloodTime::GetInstance().DeltaTime;
		transform.SetLocalPosition(transform.GetLocalPosition() + movement);
	}

	// std::cout << "FPS: " << 1.0f / BloodTime::GetInstance().DeltaTime << " With enemy count: " << result.EntityViews.size() << std::endl;
}

void EnemySpawnSystem::SpawnLoop()
{
	Entity& functionInvoker = EntityManager::GetInstance().CreateEntity();
	FunctionInvokeComponent* functionInvokeComp = EntityManager::GetInstance().AddComponent<FunctionInvokeComponent>(functionInvoker);
	functionInvokeComp->Function = [this]()
	{
		SpawnEnemy();
		SpawnLoop();
	};
	functionInvokeComp->TimeToInvoke = 0.02f;
}

void EnemySpawnSystem::SpawnEnemy()
{
	EntityManager& entityManager = EntityManager::GetInstance();

	Entity& enemyEntity = entityManager.CreateEntity();
	enemyEntity.Name = "Enemy";
	enemyEntity.Tag = CreateId("Enemy");
	Enemy* enemy = entityManager.AddComponent<Enemy>(enemyEntity);
	enemy->Speed = 40.0f;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0.0, 2 * std::numbers::pi);

	double angle = dist(gen);
	float x = m_SpawnRadius * static_cast<float>(std::cos(angle)) + BloodRenderer::GetInstance().GetWindowWidth() / 2.0f;
	float y = m_SpawnRadius * static_cast<float>(std::sin(angle)) + BloodRenderer::GetInstance().GetWindowHeight() / 2.0f;

	TransformComponent* transform = entityManager.GetComponent<TransformComponent>(enemyEntity);
	transform->SetLocalPosition(x, y);
	transform->SetLocalScale({0.25f, 0.25f});

	entityManager.AddComponent<SpriteComponent>(enemyEntity);
	SpriteAnimatorComponent* animator = entityManager.AddComponent<SpriteAnimatorComponent>(enemyEntity);
	AnimationData data;
	data.FrameTime = 0.1f;
	data.NumberOfFrames = 11;
	data.ShouldLoop = true;
	data.Texture = ResourceManager::GetInstance().LoadTexture("BatSheet.png");
	animator->AddAnimation(CreateId("Bat"), data);
	animator->PlayAnimation(CreateId("Bat"));

	RectColliderComponent* rectComp = entityManager.AddComponent<RectColliderComponent>(enemyEntity);
	rectComp->SetSize({ 100.0f, 45.0f });
	rectComp->IgnoreTags.insert(CreateId("Bat"));
	rectComp->SetOffset({-10, 17});

	Health* health = entityManager.AddComponent<Health>(enemyEntity);
	health->OnDeathEvent.AddListener([](int enemyEntityId)
		{
			AchievementMonitor::GetInstance().OnEnemyDeath();
			EntityManager::GetInstance().DestroyEntity(enemyEntityId);
		});
	health->MaxHealth = 5.0f;
	health->CurrentHealth = 5.0f;
}