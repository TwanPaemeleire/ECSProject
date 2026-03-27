#include "pch.h"
#include "SpriteSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"
#include "BloodTime.h"
#include "InputHandler.h"
#include "SdbmHash.h"
#include <SDL3/SDL.h>

namespace Bloodforge
{
	void SpriteSystem::OnRender()
	{
		EntityQueryResult result =  EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < view.GetComponentArray<TransformComponent>().size(); ++i)
			{
				Vector2 worldPos = view.GetComponentArray<TransformComponent>()[i].GetWorldPosition();
				BloodRenderer::GetInstance().RenderTexture(*view.GetComponentArray<SpriteComponent>()[i].Texture, worldPos.X, worldPos.Y);
			}
		}
	}
}