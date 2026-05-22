#pragma once
#include <System.h>
#include <unordered_set>

namespace Bloodforge
{
	struct TransformComponent;
}

class PlayerTowerSystem final : public Bloodforge::System
{	
public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;
private:
	void ShootProjectile(Bloodforge::TransformComponent* towerTransform);
	void OnTowerDeath(int entityId);

	void OnEnemyEnterRange(int selfId, int otherId);
	void OnEnemyDeath(int entityId);
};