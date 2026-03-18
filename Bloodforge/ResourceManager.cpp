#include "pch.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <memory>

namespace Bloodforge
{
	Texture2D* Bloodforge::ResourceManager::LoadTexture(const std::string& file)
	{
		if (m_LoadedTextures.find(file) == m_LoadedTextures.end())
			m_LoadedTextures.insert(std::pair(file, std::move(std::make_unique<Texture2D>(file))));
		return m_LoadedTextures.at(file).get();
	}
}