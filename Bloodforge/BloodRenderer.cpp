#include "pch.h"
#include "BloodRenderer.h"
#include <SDL3/SDL_render.h>
#include <stdexcept>
#include "Texture2D.h"
#include "SceneManager.h"
#include "Vector2.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include "glm/glm.hpp"
#include "RectColliderComponent.h"
#include "Rect.h"

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
		const Color& color = GetBackgroundColor();
		SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(m_Renderer);

		SceneManager::GetInstance().Render();
		SceneManager::GetInstance().RenderUI();

		SDL_RenderPresent(m_Renderer);
	}

	void BloodRenderer::DrawRectangle(const Vector2& pos, float width, float height, const Color& color) const
	{
		SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
		Vector2 screenPos = pos;
		SDL_FRect rect{};
		rect.x = screenPos.X;
		rect.y = screenPos.Y;
		rect.w = width;
		rect.h = height;

		SDL_RenderRect(m_Renderer, &rect);
	}

	void BloodRenderer::DrawRectangle(const ColliderRect& rect, const Color& color) const
	{
		SDL_FPoint linePoints[5] = 
		{
			{ rect.TopLeft.X, rect.TopLeft.Y },
			{ rect.TopRight.X, rect.TopRight.Y },
			{ rect.BottomRight.X, rect.BottomRight.Y },
			{ rect.BottomLeft.X, rect.BottomLeft.Y },
			{ rect.TopLeft.X, rect.TopLeft.Y }
		};

		SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderLines(GetSDLRenderer(), linePoints, 5);
	}

	void BloodRenderer::RenderTexture(const Texture2D& texture, const Vector2& pos, const Color& color) const
	{
		Vector2 screenPos = pos;
		SDL_FRect dstRect{};
		dstRect.x = screenPos.X;
		dstRect.y = screenPos.Y;
		dstRect.w = texture.GetWidth();
		dstRect.h = texture.GetHeight();
		SDL_SetTextureColorMod(texture.GetSDLTexture(), color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(texture.GetSDLTexture(), color.a);
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dstRect);
	}

	void BloodRenderer::RenderTexture(const Texture2D& texture, const glm::mat4& worldMatrix, const Color& color, bool flipHorizontal, bool flipVertical) const
	{
		const float width = texture.GetWidth();
		const float height = texture.GetHeight();
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;

		(flipHorizontal) ? halfWidth *= -1 : halfWidth;
		(flipVertical) ? halfHeight *= -1 : halfHeight;

		SDL_Vertex vertices[4]{};
		SDL_FColor sdlColor = 
		{ 
			color.r / 255.f, 
			color.g / 255.f, 
			color.b / 255.f, 
			color.a / 255.f 
		};

		vertices[0].position = TransformPoint(worldMatrix, -halfWidth, -halfHeight);
		vertices[0].tex_coord = SDL_FPoint{ 0.0f, 0.0f };
		vertices[0].color = sdlColor;

		vertices[1].position = TransformPoint(worldMatrix, halfWidth, -halfHeight);
		vertices[1].tex_coord = SDL_FPoint{ 1.0f, 0.0f };
		vertices[1].color = sdlColor;

		vertices[2].position = TransformPoint(worldMatrix, halfWidth, halfHeight);
		vertices[2].tex_coord = SDL_FPoint{ 1.0f, 1.0f };
		vertices[2].color = sdlColor;

		vertices[3].position = TransformPoint(worldMatrix, -halfWidth, halfHeight);
		vertices[3].tex_coord = SDL_FPoint{ 0.0f, 1.0f };
		vertices[3].color = sdlColor;

		int indices[6] = { 0, 1, 2, 0, 2, 3 };

		SDL_RenderGeometry(GetSDLRenderer(), texture.GetSDLTexture(), vertices, 4, indices, 6);
	}

	void BloodRenderer::RenderTexture(const Texture2D& texture, const glm::mat4& worldMatrix, const Rect& srcRect, const Color& color, bool flipHorizontal, bool flipVertical) const
	{
		const float textureWidth = texture.GetWidth();
		const float textureHeight = texture.GetHeight();

		const float width = srcRect.Width;
		const float height = srcRect.Height;
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;

		(flipHorizontal) ? halfWidth *= -1 : halfWidth;
		(flipVertical) ? halfHeight *= -1 : halfHeight;

		const float u0 = srcRect.X / textureWidth;
		const float v0 = srcRect.Y / textureHeight;
		const float u1 = (srcRect.X + srcRect.Width) / textureWidth;
		const float v1 = (srcRect.Y + srcRect.Height) / textureHeight;

		SDL_Vertex vertices[4]{};
		SDL_FColor sdlColor =
		{
			color.r / 255.f,
			color.g / 255.f,
			color.b / 255.f,
			color.a / 255.f
		};

		vertices[0].position = TransformPoint(worldMatrix, -halfWidth, -halfHeight);
		vertices[0].tex_coord = SDL_FPoint{ u0, v0 };
		vertices[0].color = sdlColor;

		vertices[1].position = TransformPoint(worldMatrix, halfWidth, -halfHeight);
		vertices[1].tex_coord = SDL_FPoint{ u1, v0 };
		vertices[1].color = sdlColor;

		vertices[2].position = TransformPoint(worldMatrix, halfWidth, halfHeight);
		vertices[2].tex_coord = SDL_FPoint{ u1, v1 };
		vertices[2].color = sdlColor;

		vertices[3].position = TransformPoint(worldMatrix, -halfWidth, halfHeight);
		vertices[3].tex_coord = SDL_FPoint{ u0, v1 };
		vertices[3].color = sdlColor;

		int indices[6] = { 0, 1, 2, 0, 2, 3 };

		SDL_RenderGeometry(GetSDLRenderer(), texture.GetSDLTexture(), vertices, 4, indices, 6);
	}

	SDL_Renderer* BloodRenderer::GetSDLRenderer() const
	{
		if (!m_Renderer) throw std::runtime_error("Renderer not initialized yet!");
		return m_Renderer;
	}

	SDL_FPoint BloodRenderer::TransformPoint(const glm::mat4& worldMatrix, float x, float y) const
	{
		glm::vec4 point = worldMatrix * glm::vec4(x, y, 0.0f, 1.0f);
		return SDL_FPoint{ point.x, point.y };
	}
}