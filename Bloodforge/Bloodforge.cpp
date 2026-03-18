#include "pch.h"
#include "Bloodforge.h"
#include <chrono>
#include <thread>
#include "BloodTime.h"
#include "BloodRenderer.h"
#include "SceneManager.h"
#include <iostream>

namespace Bloodforge
{
	void Bloodforge::Run()
	{
		auto& sceneManager = SceneManager::GetInstance();
		auto& renderer = BloodRenderer::GetInstance();
		auto& bloodTime = BloodTime::GetInstance();
		
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.f;
		
		sceneManager.Start();
		while (m_ShouldContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			bloodTime.DeltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

			lastTime = currentTime;
			lag += bloodTime.DeltaTime;
		
			while (lag >= m_FixedTimeStep)
			{
				sceneManager.FixedUpdate();
				lag -= m_FixedTimeStep;
			}
			sceneManager.Update();
			sceneManager.LateUpdate();
			renderer.Render();

			const auto sleepTime = std::chrono::milliseconds(m_MsPerFrame) - (std::chrono::high_resolution_clock::now() - currentTime);
			std::this_thread::sleep_for(sleepTime);
		}
	}
	void Bloodforge::RequestStop()
	{
		m_ShouldContinue = false;
	}
}