#pragma once
#include <chrono>

namespace Bloodforge
{
	class SpeedTracker
	{
	public:
		SpeedTracker() = default;
		~SpeedTracker() = default;

		void StartTracking();
		float StopTracking();
	private:
		std::chrono::high_resolution_clock::time_point m_startTime;
	};
}