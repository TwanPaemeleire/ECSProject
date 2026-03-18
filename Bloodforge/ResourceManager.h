#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace Bloodforge
{
	class Texture2D;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		Texture2D* LoadTexture(const std::string& file);

	protected:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

	private:
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_LoadedTextures;
	};
}					