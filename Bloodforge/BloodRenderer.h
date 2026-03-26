#pragma once
#include "Singleton.h"
#include <SDL3/SDL.h>

namespace Bloodforge
{
	class Texture2D;
	class BloodRenderer : public Singleton<BloodRenderer>
	{
	public:
		virtual void InitializeBeforeFirstUse() override;
		void Render() const;
		void Destroy();

		void Render();
		void RenderTexture(const Texture2D& texture, float x, float y) const;

		SDL_Renderer* GetSDLRenderer() const;
		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		SDL_Window* GetSDLWindow() const { return m_Window; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		int GetWindowWidth() const { return m_WindowWidth; }
		int GetWindowHeight() const { return m_WindowHeight; }
	private:
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};

		int m_WindowWidth{};
		int m_WindowHeight{};
	};
}