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
using namespace Bloodforge;

void EnemySpawnSystem::OnStart()
{
	SpawnLoop();
}

void EnemySpawnSystem::OnUpdate()
{
	EntityQueryResult result = EntityManager::GetInstance().QueryEntities<Enemy, TransformComponent>();

	int entityCount = 0;
	for (ChunkView<Enemy, TransformComponent>& chunk : result.Chunks)
	{
		std::span<Enemy> enemies = chunk.GetComponentArray<Enemy>();
		std::span<TransformComponent> transforms = chunk.GetComponentArray<TransformComponent>();
		for (size_t i = 0; i < chunk.Entities.size(); ++i)
		{
			Enemy& enemy = enemies[i];
			TransformComponent& transform = transforms[i];
			Vector2 directionToCenter = Vector2(BloodRenderer::GetInstance().GetWindowWidth() / 2.0f, BloodRenderer::GetInstance().GetWindowHeight() / 2.0f) - transform.GetLocalPosition();
			enemy.Direction = directionToCenter.Normalized();
			Vector2 movement = enemy.Direction * enemy.Speed * BloodTime::GetInstance().DeltaTime;
			transform.SetLocalPosition(transform.GetLocalPosition() + movement);
			++entityCount;
		}
	}

	std::cout << "FPS: " << 1.0f / BloodTime::GetInstance().DeltaTime << " With enemy count: " << entityCount << std::endl;
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
	Enemy* enemy = entityManager.AddComponent<Enemy>(enemyEntity);
	InitializeEnemy1(enemy);

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

	enemyEntity.Tag = CreateId("Bat");
	RectColliderComponent* rectComp = entityManager.AddComponent<RectColliderComponent>(enemyEntity);
	rectComp->SetSize({ 60.0f, 60.0f });
	// rectComp->IgnoreTags.emplace_back(CreateId("Bat"));
}

void EnemySpawnSystem::InitializeEnemy1(Enemy* enemy)
{
	enemy->Health = 1.0f;
	enemy->Speed = 20.0f;
}