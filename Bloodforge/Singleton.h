#pragma once

template <typename Type>
class Singleton
{
public:
	static Type& GetInstance()
	{
		if (!m_Initialized)
		{
			m_Initialized = true;
			InitializeBeforeFirstUse();
		}
		return m_Instance;
	}

	virtual void InitializeBeforeFirstUse() {}

private:
	static Type m_Instance;
	bool m_Initialized = false;
};