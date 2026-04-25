#pragma once
#include <System.h>
#include <stack>

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

	std::stack<int> m_EnemiesToShoot;
};