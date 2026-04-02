#pragma once
#include "Singleton.h"
#include "Color.h"

struct SDL_Renderer;
struct SDL_Window;

namespace Bloodforge
{
	struct Vector2;
	class Texture2D;

	class BloodRenderer : public Singleton<BloodRenderer>
	{
	public:
		virtual void InitializeBeforeFirstUse() override;
		void Render() const;
		void Destroy();

		void Render();

		void DrawRectangle(const Vector2& pos, float width, float height, const Color& color) const;
		void RenderTexture(const Texture2D& texture, const Vector2& pos, const Color& color) const;

		SDL_Renderer* GetSDLRenderer() const;
		const Color& GetBackgroundColor() const { return m_ClearColor; }
		SDL_Window* GetSDLWindow() const { return m_Window; }
		void SetBackgroundColor(const Color& color) { m_ClearColor = color; }

		int GetWindowWidth() const { return m_WindowWidth; }
		int GetWindowHeight() const { return m_WindowHeight; }
	private:

		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		Color m_ClearColor{0,0,0,255};

		int m_WindowWidth{};
		int m_WindowHeight{};
	};
}