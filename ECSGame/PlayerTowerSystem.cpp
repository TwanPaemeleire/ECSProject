#include "PlayerTowerSystem.h"
#include <EntityManager.h>
#include "Health.h"
#include "PlayerTower.h"
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
	EntityQueryResult<PlayerTower, Health, RectColliderComponent> result = entityManager.QueryEntities<PlayerTower, Health, RectColliderComponent>();

	for (EntityView<PlayerTower, Health, RectColliderComponent>& entityView : result.EntityViews)
	{
		Health& health = entityView.GetComponent<Health>();
		health.OnDeathEvent.AddListener([this](int entityId)
			{
				OnTowerDeath(entityId);
			});
		RectColliderComponent& rectCollider = entityView.GetComponent<RectColliderComponent>();
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

void PlayerTowerSystem::OnUpdate()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityQueryResult<PlayerTower, Health> result = entityManager.QueryEntities<PlayerTower, Health>();

	for (EntityView<PlayerTower, Health>& entityView : result.EntityViews)
	{
		PlayerTower& tower = entityView.GetComponent<PlayerTower>();
		tower.TimeUntilNextShot -= BloodTime::GetInstance().DeltaTime;
		if (tower.TimeUntilNextShot <= 0.0f && !tower.EnemiesToShoot.empty())
		{
			TransformComponent* towerTransform = entityManager.GetComponent<TransformComponent>(entityView.EntityId);
			ShootProjectile(towerTransform);
			tower.TimeUntilNextShot = tower.FireRateDelay;
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

	EntityView<PlayerTower> towerEntityView = entityManager.GetFirstEntityWithComponents<PlayerTower>().value();
	PlayerTower& towerComp = towerEntityView.GetComponent<PlayerTower>();
	int enemyToShoot = *towerComp.EnemiesToShoot.begin();
	towerComp.EnemiesToShoot.erase(enemyToShoot);
	TransformComponent* enemyTransform = entityManager.GetComponent<TransformComponent>(enemyToShoot);
	Vector2 enemyWorldPos = enemyTransform->GetWorldPosition();
	Vector2 dir = enemyWorldPos - transformComp->GetWorldPosition();
	projectileComp->Direction = dir.Normalized();

	SpriteComponent* spriteComp = entityManager.AddComponent<SpriteComponent>(projectileEntity);
	spriteComp->SetTexture(ResourceManager::GetInstance().LoadTexture("Tower/TestProjectile.png"));
	spriteComp->DrawOrder = 1;

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
	EntityView<PlayerTower> towerEntityView = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerTower>().value();
	PlayerTower& towerComp = towerEntityView.GetComponent<PlayerTower>();
	towerComp.EnemiesToShoot.insert(otherId);
}

void PlayerTowerSystem::OnEnemyDeath(int entityId)
{
	EntityView<PlayerTower> towerEntityView = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerTower>().value();
	PlayerTower& towerComp = towerEntityView.GetComponent<PlayerTower>();
	if (towerComp.EnemiesToShoot.contains(entityId))
	{
		towerComp.EnemiesToShoot.erase(entityId);
	}
}