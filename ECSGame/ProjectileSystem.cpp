#include "ProjectileSystem.h"
#include <EntityManager.h>
#include "Projectile.h"
#include <BloodTime.h>
#include <RectColliderComponent.h>
#include "Health.h"
using namespace Bloodforge;

void ProjectileSystem::OnUpdate()
{
	EntityQueryResult result = EntityManager::GetInstance().QueryEntities<Projectile, TransformComponent>();
	for (ChunkView<Projectile, TransformComponent>& chunk : result.Chunks)
	{
		std::span<Projectile> projectiles = chunk.GetComponentArray<Projectile>();
		std::span<TransformComponent> transforms = chunk.GetComponentArray<TransformComponent>();
		for (size_t i = 0; i < chunk.Entities.size(); i++)
		{
			Projectile& projectile = projectiles[i];
			if (!projectile.Registered)
			{
				RegisterProjectile(EntityManager::GetInstance().GetEntity(chunk.Entities[i]));
				projectile.Registered = true;
			}
			TransformComponent& transform = transforms[i];
			Vector2 movement = projectile.Direction * projectile.Speed * BloodTime::GetInstance().DeltaTime;
			transform.SetLocalPosition(transform.GetLocalPosition() + movement);
		}
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