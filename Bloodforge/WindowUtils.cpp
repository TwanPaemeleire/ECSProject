#include "pch.h"
#include "WindowUtils.h"
#include <SDL3/SDL.h>
#include "BloodRenderer.h"
#include "CustomCursor.h"
#include <SDL3/SDL_mouse.h>
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

namespace Bloodforge::WindowUtils
{
	void SetWindowSize(int sizeX, int sizeY)
	{
		BloodRenderer::GetInstance().SetWindowSize(sizeX, sizeY);
	}

	void SetWindowAlwaysOnTop(bool alwaysOnTop)
	{
		SDL_SetWindowAlwaysOnTop(BloodRenderer::GetInstance().GetSDLWindow(), alwaysOnTop);
	}

	void SetWindowBordered(bool borderless)
	{
		SDL_SetWindowBordered(BloodRenderer::GetInstance().GetSDLWindow(), borderless);
	}

	void SetWindowFullScreen(bool fullscreen)
	{
		SDL_SetWindowFullscreen(BloodRenderer::GetInstance().GetSDLWindow(), fullscreen);
	}

	void SetCustomCursor(CustomCursor* customCursor)
	{
		SDL_SetCursor(customCursor->GetSDLCursor());
	}

	void SetWindowIcon(const std::string& path)
	{
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface)
		{
			throw std::runtime_error(SDL_GetError());
		}

		SDL_SetWindowIcon(BloodRenderer::GetInstance().GetSDLWindow(), surface);
		SDL_DestroySurface(surface);
	}
}

