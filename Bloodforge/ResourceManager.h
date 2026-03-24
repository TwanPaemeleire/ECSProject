#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <memory>
#include "Font.h"

namespace std
{
	template<>
	struct hash<std::pair<std::string, float>>
	{
		size_t operator()(const std::pair<std::string, float>& p) const noexcept
		{
			size_t h1 = hash<std::string>{}(p.first);
			size_t h2 = hash<float>{}(p.second);
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
		void SetResourcesDirectory(const std::string& path) const;
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, float size);

	protected:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

	private:
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_LoadedTextures;
		std::unordered_map<std::pair<std::string, float>, std::unique_ptr<Font>> m_LoadedFonts;
	};
}					