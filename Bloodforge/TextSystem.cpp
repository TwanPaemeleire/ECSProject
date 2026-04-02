#include "pch.h"
#include "TextSystem.h"
#include "EntityManager.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"

namespace Bloodforge
{
	void TextSystem::OnUpdate()
	{
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<TextComponent, TransformComponent>();

		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < view.GetComponentArray<TextComponent>().size(); ++i)
			{
				TextComponent& textComp = view.GetComponentArray<TextComponent>()[i];
				if (textComp.TextDoesNeedUpdate())
				{
					const Color& color = textComp.GetColor();
					SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

					if (textComp.FontDoesNeedUpdate())
					{
						textComp.SetFont(ResourceManager::GetInstance().LoadFont(textComp.GetFont()->GetFontPath(), textComp.GetFontSize()));
					}

					const auto surf = TTF_RenderText_Blended(textComp.GetFont()->GetFontPointer(), textComp.GetText().c_str(), 0, sdlColor);
					if (surf == nullptr)
					{
						throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
					}
					const auto texture = SDL_CreateTextureFromSurface(BloodRenderer::GetInstance().GetSDLRenderer(), surf);
					if (texture == nullptr)
					{
						throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
					}
					SDL_DestroySurface(surf);
					textComp.SetTexture(std::make_unique<Texture2D>(texture));
					textComp.CompleteUpdate();
				}
			}
		}
	}
	void TextSystem::OnRender()
	{
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<TextComponent, TransformComponent>();

		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < view.GetComponentArray<TransformComponent>().size(); ++i)
			{
				TextComponent& textComp = view.GetComponentArray<TextComponent>()[i];
				Vector2 worldPos = view.GetComponentArray<TransformComponent>()[i].GetWorldPosition();
				const Vector2& size = textComp.GetTexture()->GetSize();
				worldPos.X -= size.X / 2.0f;
				worldPos.Y -= size.Y / 2.0f;
				BloodRenderer::GetInstance().RenderTexture(*textComp.GetTexture(), worldPos, textComp.GetColor());
			}
		}
	}
}