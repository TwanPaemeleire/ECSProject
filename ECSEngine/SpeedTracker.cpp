#include "pch.h"
#include "SpeedTracker.h"

void SpeedTracker::StartTracking()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

float SpeedTracker::StopTracking()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime);
	return duration.count() / 1000.0f;
}