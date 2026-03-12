#pragma once
#include <unordered_map>
#include "Component.h"

struct ComponentInfo
{
	size_t Size;
};

class ComponentRegistry
{
public:
	ComponentRegistry() = default;
	~ComponentRegistry() = default;

	template <typename ComponentType>
	bool TryRegisterComponent();

	template <typename ComponentType>
	const ComponentInfo& GetComponentInfo();
	const ComponentInfo& GetComponentInfo(int componentId);
private:
	std::unordered_map<int, ComponentInfo> m_ComponentRegistry;
	int m_ComponentIndexTracker = 0;
};

template<typename ComponentType>
inline bool ComponentRegistry::TryRegisterComponent()
{
	if(Component<ComponentType>::Index != -1) return false;
	Component<ComponentType>::Index = m_ComponentIndexTracker++;

	m_ComponentRegistry[Component<ComponentType>::Index] = {sizeof(ComponentType)};
	return true;
}

template<typename ComponentType>
inline const ComponentInfo& ComponentRegistry::GetComponentInfo()
{
	TryRegisterComponent<ComponentType>();
	return m_ComponentRegistry[Component<ComponentType>::Index];
}
