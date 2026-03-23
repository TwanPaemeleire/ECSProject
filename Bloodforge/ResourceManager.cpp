#include "pch.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>
#include <iostream>

namespace Bloodforge
{
	void ResourceManager::InitializeBeforeFirstUse()
	{
		TTF_Init();
	}

	Texture2D* Bloodforge::ResourceManager::LoadTexture(const std::string& file)
	{
		if (m_LoadedTextures.find(file) == m_LoadedTextures.end())
			m_LoadedTextures.insert(std::pair(file, std::move(std::make_unique<Texture2D>(file))));
		return m_LoadedTextures.at(file).get();
	}

	TTF_Font* ResourceManager::LoadFont(const std::string& file, uint8_t size)
	{
		const auto key = std::pair<std::string, uint8_t>(file, size);
		if (m_LoadedFonts.find(key) == m_LoadedFonts.end())
		{
			TTF_Font* font = TTF_OpenFont(file.c_str(), size);
			if (font == nullptr)
			{
				throw std::runtime_error(std::string("Loading font failed: ") + SDL_GetError());
			}
			m_LoadedFonts.insert(std::pair(key, font));
		}
		return m_LoadedFonts.at(key);
	}
}