#pragma once
#include "Singleton.h"

namespace Bloodforge
{
	class BloodTime final : public Singleton<BloodTime>
	{
	public:
		float DeltaTime{};
	private:
		friend class Singleton<BloodTime>;
		BloodTime() = default;
	};
}