#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace std
{
	template<>
	struct hash<std::pair<std::string, uint8_t>>
	{
		size_t operator()(const std::pair<std::string, uint8_t>& p) const noexcept
		{
			size_t h1 = hash<std::string>{}(p.first);
			size_t h2 = hash<uint8_t>{}(p.second);
			return h1 ^ (h2 << 1);
		}
	};
}

struct TTF_Font;
namespace Bloodforge
{
	class Texture2D;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		virtual void InitializeBeforeFirstUse() override;
		Texture2D* LoadTexture(const std::string& file);
		TTF_Font* LoadFont(const std::string& file, uint8_t size);

	protected:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

	private:
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_LoadedTextures;
		std::unordered_map<std::pair<std::string, uint8_t>, TTF_Font*> m_LoadedFonts;
	};
}					