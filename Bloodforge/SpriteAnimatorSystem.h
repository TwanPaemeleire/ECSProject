#pragma once
#include "System.h"
#include <functional>
#include <unordered_map>

namespace Bloodforge
{
	struct SpriteAnimatorComponent;

	class SpriteAnimatorSystem final : public System
	{
	public:
		virtual void OnStart() override;
		virtual void OnUpdate() override;

		int AddAnimationEvent(std::function<void(SpriteAnimatorComponent&)> callback);

	private:
		void HandleAnimationEvents(SpriteAnimatorComponent& spriteAnimComp);
		void ResetAllAnimationEvents(SpriteAnimatorComponent& spriteAnimComp);
		std::unordered_map<int, std::function<void(SpriteAnimatorComponent&)>> m_AnimationEventsCallbacks;
		int m_NextAnimationEventCallbackID = 0;
	};
}