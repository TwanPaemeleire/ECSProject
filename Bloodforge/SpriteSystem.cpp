#include "pch.h"
#include "SpriteSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"
#include "Texture2D.h"

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
				Vector2 worldPos = view.GetComponentArray<TransformComponent>()[i].GetWorldPosition();
				const Vector2& size = spriteComp.Texture->GetSize();
				worldPos.X -= size.X / 2.0f;
				worldPos.Y -= size.Y / 2.0f;
				BloodRenderer::GetInstance().RenderTexture(*spriteComp.Texture, worldPos, spriteComp.Color);
			}
		}
	}
}