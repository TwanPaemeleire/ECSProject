#pragma once
#include <string>
#include <vector>

using ArchetypeIdentifier = std::vector<int>;
struct ArchetypeIdentifierHash
{
	size_t operator()(const ArchetypeIdentifier& ids) const noexcept
	{
		std::size_t seed = ids.size();
		for (auto x : ids) {
			seed ^= std::hash<int>{}(x)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

class EntityManager;

struct Entity
{
	Entity(int id);
	~Entity() = default;

	void SetCurrentChunk(const ArchetypeIdentifier& archetypeIds, int chunkIndex);
	template <typename Component>
	void AddComponent();

	int Id = -1;
	ArchetypeIdentifier CurrentArchetypeIds;
	int CurrentChunkIndex = -1;
};