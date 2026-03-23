#pragma once
#include "Component.h"
#include <string>
#include "Color.h"
#include <memory>
#include "Texture2D.h"

struct TTF_Font;
struct SDL_Color;
namespace Bloodforge
{
	struct TextComponent final : public Component<TextComponent>
	{
		void SetText(const std::string& text)
		{
			Text = text;
			NeedsUpdate = true;
		}

		void SetFontSize(int size)
		{
			FontSize = size;
			NeedsUpdate = true;
		}

		void SetColor(const Color& color)
		{
			Color = color;
			NeedsUpdate = true;
		}

		void SetFont(TTF_Font* font)
		{
			Font = font;
			NeedsUpdate = true;
		}

		void SetTexture(std::unique_ptr<Texture2D> texture)
		{
			Texture = std::move(texture);
			NeedsUpdate = false;
		}

		void CompleteUpdate()
		{
			NeedsUpdate = false;
		}

		bool DoesNeedUpdate() const { return NeedsUpdate; }
		Texture2D* GetTexture() const { return Texture.get(); }
		TTF_Font* GetFont() const { return Font; }
		int GetFontSize() const { return FontSize; }
		const std::string& GetText() const { return Text; }
		const Color& GetColor() const { return Color; }

	private:
		std::string Text;
		int FontSize;
		Color Color;
		TTF_Font* Font;
		std::unique_ptr<Texture2D> Texture;
		bool NeedsUpdate = true;
	};
}