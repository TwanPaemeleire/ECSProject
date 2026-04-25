#pragma once
#include <System.h>

namespace Bloodforge
{
	struct Entity;
}

class ProjectileSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
	void RegisterProjectile(Bloodforge::Entity& projectileEntity);
private:
	void OnProjectileHit(int selfId, int otherId);
};