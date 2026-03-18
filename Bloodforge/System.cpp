#include "pch.h"
#include "System.h"

namespace Bloodforge
{
	System::~System() = default;

	void System::Enable()
	{
		m_IsEnabled = true;
		OnEnable();
	}

	void System::Disable()
	{
		m_IsEnabled = false;
		OnDisable();
	}
}