#include "pch.h"
#include "SpriteSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"
#include "Texture2D.h"
#include "Rect.h"

namespace Bloodforge
{
	void SpriteSystem::OnRender()
	{
		EntityQueryResult result =  EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < view.GetComponentArray<TransformComponent>().size(); ++i)
			{
				SpriteComponent& spriteComp = view.GetComponentArray<SpriteComponent>()[i];
				TransformComponent& transform = view.GetComponentArray<TransformComponent>()[i];
				if (spriteComp.GetTexture() == nullptr) continue;
				BloodRenderer::GetInstance().RenderTexture(*spriteComp.GetTexture(), transform.GetWorldMatrix(), spriteComp.GetSourceRect(), spriteComp.Color, spriteComp.FlipHorizontal, spriteComp.FlipVertical);
			}
		}
	}
}