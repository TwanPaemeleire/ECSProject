#pragma once
#include "System.h"
#include <functional>
#include <unordered_map>

namespace Bloodforge
{
	struct SpriteAnimationComponent;

	class SpriteAnimationSystem final : public System
	{
	public:
		virtual void OnStart() override;
		virtual void OnUpdate() override;

		int AddAnimationEvent(std::function<void()> callback);

	private:
		void HandleAnimationEvents(SpriteAnimationComponent& spriteAnimComp);
		void ResetAllAnimationEvents(SpriteAnimationComponent& spriteAnimComp);
		std::unordered_map<int, std::function<void()>> m_AnimationEventsCallbacks;
		int m_NextAnimationEventCallbackID = 0;
	};
}