#pragma once
#include <chrono>

class SpeedTracker
{
public:
	SpeedTracker() = default;
	~SpeedTracker() = default;

	void StartTracking();
	int StopTracking();
private:
	std::chrono::high_resolution_clock::time_point m_startTime;
};