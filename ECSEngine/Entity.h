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
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = (x >> 16) ^ x;
			seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

class EntityManager;

class Entity
{
public:
	Entity(int id);
	~Entity() = default;

	void SetCurrentChunk(const ArchetypeIdentifier& archetypeIds, int chunkIndex);

	int GetId() const { return m_Id; }
	const ArchetypeIdentifier& GetCurrentArchetypeIds() const { return m_CurrentArchetypeIds; }
	int GetCurrentChunkIndex() const { return m_CurrentChunkIndex; }

	template <typename Component>
	void AddComponent();

private:
	int m_Id = -1;
	ArchetypeIdentifier m_CurrentArchetypeIds;
	int m_CurrentChunkIndex = -1;
};