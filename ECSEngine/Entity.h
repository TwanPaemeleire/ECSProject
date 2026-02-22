#pragma once
#include <string>

class Entity
{
public:
	Entity(int id);
	~Entity() = default;

	void SetCurrentChunk(const std::string& chunkId, int chunkIndex, int chunkEntityIndex);

	int GetId() const { return m_Id; }
	const std::string& GetCurrentChunkId() const { return m_CurrentChunkId; }
	int GetCurrentChunkIndex() const { return m_CurrentChunkIndex; }
	int GetCurrentChunkEntityIndex() const { return m_CurrentChunkEntityIndex; }

private:
	int m_Id = -1;
	std::string m_CurrentChunkId = "";
	int m_CurrentChunkIndex = -1;
	int m_CurrentChunkEntityIndex = -1; // Index of the entity within the current chunk
};