#include "pch.h"
#include "ComponentRegistry.h"

const ComponentInfo& ComponentRegistry::GetComponentInfo(int componentId)
{
    return m_ComponentRegistry[componentId];
}
