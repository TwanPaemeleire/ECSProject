#pragma once
#include <Component.h>
#include <Vector2.h>

struct Projectile final : public Bloodforge::Component<Projectile>
{
	float Speed = 50.0f;
	float Damage = 10.0f;
	Bloodforge::Vector2 Direction = { 0.0f, 0.0f };
	bool Registered = false;
};