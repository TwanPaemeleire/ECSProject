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
		EntityQueryResult<TransformComponent, SpriteComponent> result = EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (EntityView<TransformComponent, SpriteComponent>& view : result.EntityViews)
		{
			TransformComponent& transform = view.GetComponent<TransformComponent>();
			SpriteComponent& spriteComp = view.GetComponent<SpriteComponent>();
			if (spriteComp.GetTexture() == nullptr) continue;
			BloodRenderer::GetInstance().RenderTexture(*spriteComp.GetTexture(), transform.GetWorldMatrix(), spriteComp.GetSourceRect(), spriteComp.Color, spriteComp.FlipHorizontal, spriteComp.FlipVertical);
		}
	}
}