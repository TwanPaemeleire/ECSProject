#pragma once
#include <System.h>
#include <Event.h>
#include <memory>

struct TestGlobalEventData
{
	int Test = 0;
};

class TestSystem final : public Bloodforge::System
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;

private:
	std::unique_ptr<Bloodforge::Event<float, int>> m_TestEvent;
	void GlobalEventTest(const TestGlobalEventData& eventData);
	void LocalEventTest(float x, int y);
	void OnCollisionEnterEvent(int otherEntityId);
	void OnCollisionEvent(int otherEntityId);
	void OnCollisionExitEvent(int otherEntityId);

	bool m_TestFlag = false;
};