#pragma once
#include <System.h>
#include <Event.h>
#include <memory>

class TestSystem final : public Bloodforge::System
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;
private:
	void OnCollisionStart();
	void OnCollision();
	void OnCollisionEnd();
};