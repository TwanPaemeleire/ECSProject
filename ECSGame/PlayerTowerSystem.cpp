#include "PlayerTowerSystem.h"
#include <EntityManager.h>
#include "Health.h"
#include "PlayerTower.h"
#include <SceneManager.h>
#include <BloodTime.h>
#include "Projectile.h"
#include <SpriteComponent.h>
#include <ResourceManager.h>
#include <RectColliderComponent.h>
#include <IdCreator.h>

using namespace Bloodforge;

void PlayerTowerSystem::OnStart()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityQueryResult result = entityManager.QueryEntities<PlayerTower, Health, RectColliderComponent>();
	for (ChunkView<PlayerTower, Health, RectColliderComponent>& chunk : result.Chunks)
	{
		std::span<PlayerTower> towers = chunk.GetComponentArray<PlayerTower>();
		std::span<Health> healths = chunk.GetComponentArray<Health>();
		std::span<RectColliderComponent> rectColliders = chunk.GetComponentArray<RectColliderComponent>();
		for (size_t i = 0; i < chunk.Entities.size(); i++)
		{
			Health& health = healths[i];
			health.OnDeathEvent.AddListener([this](int entityId)
				{
					OnTowerDeath(entityId);
				});
			RectColliderComponent& rectCollider = rectColliders[i];
			Entity& towerEntity = entityManager.GetEntity(chunk.Entities[i]);
			towerEntity;
			rectCollider.OnCollisionEnterEvent.AddListener([this](int selfId, int otherId)
				{
					EntityManager& entityManager = EntityManager::GetInstance();
					Entity& otherEntity = entityManager.GetEntity(otherId);
					if (otherEntity.Tag == CreateId("Enemy"))
					{
						OnEnemyEnterRange(selfId, otherId);
					}
				});
		}
	}
}

void PlayerTowerSystem::OnUpdate()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityQueryResult result = entityManager.QueryEntities<PlayerTower, Health>();
	for (ChunkView<PlayerTower, Health>& chunk : result.Chunks)
	{
		std::span<PlayerTower> towers = chunk.GetComponentArray<PlayerTower>();
		std::span<Health> healths = chunk.GetComponentArray<Health>();
		for (size_t i = 0; i < chunk.Entities.size(); i++)
		{
			PlayerTower& tower = towers[i];
			tower.TimeUntilNextShot -= BloodTime::GetInstance().DeltaTime;
			if (tower.TimeUntilNextShot <= 0.0f && !m_EnemiesToShoot.empty())
			{
				TransformComponent* towerTransform = entityManager.GetComponent<TransformComponent>(chunk.Entities[i]);
				ShootProjectile(towerTransform);
				tower.TimeUntilNextShot = tower.FireRateDelay;
			}
		}
	}
}

void PlayerTowerSystem::ShootProjectile(TransformComponent* towerTransform)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	Entity& projectileEntity = entityManager.CreateEntity();
	projectileEntity.Name = "Projectile";
	projectileEntity.Tag = CreateId("PlayerProjectile");

	TransformComponent* transformComp = entityManager.GetComponent<TransformComponent>(projectileEntity);
	transformComp->SetLocalScale({ 1.0f, 1.0f });
	transformComp->SetLocalPosition(towerTransform->GetWorldPosition().X, towerTransform->GetWorldPosition().Y - 35.0f);

	Projectile* projectileComp = entityManager.AddComponent<Projectile>(projectileEntity);
	projectileComp->Damage = 20.0f;
	projectileComp->Speed = 200.0f;

	int enemyToShoot = *m_EnemiesToShoot.begin();
	m_EnemiesToShoot.erase(enemyToShoot);
	TransformComponent* enemyTransform = entityManager.GetComponent<TransformComponent>(enemyToShoot);
	Vector2 enemyWorldPos = enemyTransform->GetWorldPosition();
	Vector2 dir = enemyWorldPos - transformComp->GetWorldPosition();
	projectileComp->Direction = dir.Normalized();

	SpriteComponent* spriteComp = entityManager.AddComponent<SpriteComponent>(projectileEntity);
	spriteComp->SetTexture(ResourceManager::GetInstance().LoadTexture("Tower/TestProjectile.png"));


	RectColliderComponent* rectCollider = entityManager.AddComponent<RectColliderComponent>(projectileEntity);
	rectCollider->SetSize({ 10.0f, 10.0f });
	rectCollider->IgnoreTags.insert(CreateId("PlayerTower"));
	rectCollider->IgnoreTags.insert(CreateId("PlayerProjectile"));
}

void PlayerTowerSystem::OnTowerDeath(int entityId)
{
	EntityManager::GetInstance().DestroyEntity(entityId);
}

void PlayerTowerSystem::OnEnemyEnterRange(int, int otherId)
{
	Health* enemyHealth = EntityManager::GetInstance().GetComponent<Health>(otherId);
	enemyHealth->OnDeathEvent.AddListener([this](int entityId)
		{
			OnEnemyDeath(entityId);
		});
	m_EnemiesToShoot.insert(otherId);
}

void PlayerTowerSystem::OnEnemyDeath(int entityId)
{
	if (m_EnemiesToShoot.contains(entityId))
	{
		m_EnemiesToShoot.erase(entityId);
	}
}