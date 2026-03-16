#include "pch.h"
#include "Bloodforge.h"
#include <chrono>
#include <thread>
#include "BloodTime.h"

namespace Bloodforge
{
	void Bloodforge::Run()
	{
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.f;

		while (m_ShouldContinue)
		{
			auto& bloodTime = BloodTime::GetInstance();

			const auto currentTime = std::chrono::high_resolution_clock::now();
			const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			bloodTime.DeltaTime = deltaTime;

			while (lag >= m_FixedTimeStep)
			{
				lag -= m_FixedTimeStep;
			}

			const auto sleepTime = currentTime + std::chrono::milliseconds(m_MsPerFrame) - std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for(sleepTime);
		}
	}
	void Bloodforge::RequestStop()
	{
		m_ShouldContinue = false;
	}
}