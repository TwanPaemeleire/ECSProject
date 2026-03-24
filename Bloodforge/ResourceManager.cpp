#include "pch.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>
#include <iostream>
#include <filesystem>

namespace Bloodforge
{
	void ResourceManager::InitializeBeforeFirstUse()
	{
		TTF_Init();
	}

	void ResourceManager::SetResourcesDirectory(const std::string& path) const
	{
		if (std::filesystem::exists(path))
		{
			std::filesystem::current_path(path);
		}
		else
		{
			throw std::runtime_error("Tried setting resources directory to a folder which doesn't exist.");
		}
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
}