#include "pch.h"
#include "BloodRenderer.h"
#include <SDL3/SDL_render.h>
#include <stdexcept>
#include "Texture2D.h"
#include "SceneManager.h"

namespace Bloodforge
{
	void BloodRenderer::InitializeBeforeFirstUse()
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
		}

		m_Window = SDL_CreateWindow("Bloodforge", 800, 800, SDL_WINDOW_OPENGL);
		if (m_Window == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}

		m_Renderer = SDL_CreateRenderer(m_Window, NULL);
		if (m_Renderer == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
		}
		SDL_GetWindowSizeInPixels(m_Window, &m_WindowWidth, &m_WindowHeight);
	}

	void Bloodforge::BloodRenderer::Render() const
	{
	}

	void Bloodforge::BloodRenderer::Destroy()
	{
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	void BloodRenderer::Render()
	{
		const auto& color = GetBackgroundColor();
		SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(m_Renderer);

		SceneManager::GetInstance().Render();
		SceneManager::GetInstance().RenderUI();

		SDL_RenderPresent(m_Renderer);
	}

	void BloodRenderer::RenderTexture(const Texture2D& texture, float x, float y) const
	{
		SDL_FRect dstRect{};
		dstRect.x = x;
		dstRect.y = y;
		dstRect.w = texture.GetSizeX();
		dstRect.h = texture.GetSizeY();
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dstRect);
	}

	SDL_Renderer* BloodRenderer::GetSDLRenderer() const
	{
		if (!m_Renderer) throw std::runtime_error("Renderer not initialized yet!");
		return m_Renderer;
	}
}