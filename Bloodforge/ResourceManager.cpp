#include "pch.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <SDL3_image/SDL_image.h>

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

	Font* ResourceManager::LoadFont(const std::string& file, float size)
	{
		const auto key = std::pair<std::string, float>(file, size);
		if (m_LoadedFonts.find(key) == m_LoadedFonts.end())
		{
			m_LoadedFonts.insert(std::pair(key, std::make_unique<Font>(file, size)));
		}
		return m_LoadedFonts.at(key).get();
	}
	CustomCursor* ResourceManager::LoadCustomCursor(CursorId id, const std::string& file, int xOffset, int yOffset)
	{
		auto it = m_LoadedCustomCursors.find(id);
		if (it != m_LoadedCustomCursors.end())
		{
			return it->second.get();
		}

		SDL_Surface* surface = IMG_Load(file.c_str());
		if (!surface)
		{
			throw std::runtime_error(SDL_GetError());
		}

		auto cursor = std::make_unique<CustomCursor>(surface, xOffset, yOffset);
		SDL_DestroySurface(surface);
		CustomCursor* customCursor = cursor.get();
		m_LoadedCustomCursors[id] = std::move(cursor);

		return customCursor;
	}
}