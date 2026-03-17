#include "pch.h"
#include "BloodRenderer.h"
#include <SDL3/SDL_render.h>
#include <stdexcept>

namespace Bloodforge
{
	void Bloodforge::BloodRenderer::Init()
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
	SDL_Renderer* BloodRenderer::GetSDLRenderer() const
	{
		return m_Renderer;
	}
}