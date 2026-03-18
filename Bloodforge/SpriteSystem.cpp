#include "pch.h"
#include "SpriteSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"
#include "BloodTime.h"

namespace Bloodforge
{
	void SpriteSystem::OnStart()
	{
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
			{
				std::get<0>(view.ComponentArrays)[i].X = static_cast<float>(i);
				std::get<0>(view.ComponentArrays)[i].Y = static_cast<float>(i);
			}
		}
	}
	void SpriteSystem::OnRender()
	{
		EntityQueryResult result =  EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
			{
				float x = std::get<0>(view.ComponentArrays)[i].X += m_Speed * BloodTime::GetInstance().DeltaTime;
				float y = std::get<0>(view.ComponentArrays)[i].Y += m_Speed * BloodTime::GetInstance().DeltaTime;
				BloodRenderer::GetInstance().RenderTexture(*std::get<1>(view.ComponentArrays)[i].Texture, x, y);
			}
		}
	}
}
