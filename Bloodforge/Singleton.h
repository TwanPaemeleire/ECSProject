#pragma once

template <typename Type>
class Singleton
{
public:
	static Type& GetInstance()
	{
		static Type instance;
		static bool initialized = false;

		if (!initialized)
		{
			initialized = true;
			instance.InitializeBeforeFirstUse();
		}

		return instance;
	}

	virtual void InitializeBeforeFirstUse() {}
};