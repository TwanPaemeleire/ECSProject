#pragma once
#include <string>

class Entity
{
public:
	Entity(int id);
	~Entity() = default;

	void SetCurrentChunk(const std::string& chunkId, int chunkIndex);

	int GetId() const { return m_Id; }
	const std::string& GetCurrentChunkId() const { return m_CurrentChunkId; }
	int GetCurrentChunkIndex() const { return m_CurrentChunkIndex; }

	template <typename Component>
	void AddComponent();

private:
	int m_Id = -1;
	std::string m_CurrentChunkId = "";
	int m_CurrentChunkIndex = -1;
};

template<typename Component>
inline void Entity::AddComponent()
{
	// Tell GameManager to move which chunk this entity is in
}