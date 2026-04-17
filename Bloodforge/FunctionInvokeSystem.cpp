#include "pch.h"
#include "FunctionInvokeSystem.h"
#include "EntityManager.h"
#include "FunctionInvokeComponent.h"
#include "BloodTime.h"

namespace Bloodforge
{
	void FunctionInvokeSystem::OnUpdate()
	{
		std::vector<int> entitiesToDestroy;
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<FunctionInvokeComponent>();
		for (ChunkView<FunctionInvokeComponent>& chunk : result.Chunks)
		{
			for (int i = 0; i < chunk.GetComponentArray<FunctionInvokeComponent>().size(); ++i)
			{
				FunctionInvokeComponent& invokeComp = chunk.GetComponentArray<FunctionInvokeComponent>()[i];
				invokeComp.TimeToInvoke -= BloodTime::GetInstance().DeltaTime;
				if (invokeComp.TimeToInvoke <= 0.0f)
				{
					invokeComp.Function();
					entitiesToDestroy.emplace_back(invokeComp.OwnerEntityId);
				}
			}
		}

		EntityManager::GetInstance().DestroyEntity(entitiesToDestroy);
	}
}