#include "ProjectileSystem.h"
#include <EntityManager.h>
#include "Projectile.h"
#include <BloodTime.h>
#include <RectColliderComponent.h>
#include "Health.h"
using namespace Bloodforge;

void ProjectileSystem::OnUpdate()
{
	EntityQueryResult<Projectile, TransformComponent> result = EntityManager::GetInstance().QueryEntities<Projectile, TransformComponent>();

	for (EntityView<Projectile, TransformComponent>& entityView : result.EntityViews)
	{
		Projectile& projectile = entityView.GetComponent<Projectile>();
		if (!projectile.Registered)
		{
			RegisterProjectile(EntityManager::GetInstance().GetEntity(entityView.EntityId));
			projectile.Registered = true;
		}
		TransformComponent& transform = entityView.GetComponent<TransformComponent>();
		Vector2 movement = projectile.Direction * projectile.Speed * BloodTime::GetInstance().DeltaTime;
		transform.SetLocalPosition(transform.GetLocalPosition() + movement);
	}
}

void ProjectileSystem::RegisterProjectile(Entity& projectileEntity)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	RectColliderComponent* rectCollider = entityManager.GetComponent<RectColliderComponent>(projectileEntity);
	rectCollider->OnCollisionEnterEvent.AddListener([this](int selfId, int otherId) 
		{ 
			OnProjectileHit(selfId, otherId); 
		});
}

void ProjectileSystem::OnProjectileHit(int selfId, int otherId)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	Health* health = entityManager.GetComponent<Health>(otherId);
	if (health)
	{
		Projectile* projectile = entityManager.GetComponent<Projectile>(selfId);
		if (projectile)
		{
			health->CurrentHealth -= projectile->Damage;
			health->OnHealthChangedEvent.Invoke(health->CurrentHealth);
			if (health->CurrentHealth <= 0.0f)
			{
				health->OnDeathEvent.Invoke(otherId);
			}
			entityManager.DestroyEntity(selfId);
		}
	}
}