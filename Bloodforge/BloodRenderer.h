#pragma once
#include "Singleton.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>

namespace Bloodforge
{

	class BloodRenderer : public Singleton<BloodRenderer>
	{
	public:
		void Init();
		void Render() const;
		void Destroy();


		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
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