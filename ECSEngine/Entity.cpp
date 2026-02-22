#include "pch.h"
#include "Entity.h"

Entity::Entity(int id)
	: m_Id(id)
{
}

void Entity::SetCurrentChunk(const std::string& chunkId, int chunkIndex, int chunkEntityIndex)
{
	m_CurrentChunkId = chunkId;
	m_CurrentChunkIndex = chunkIndex;
	m_CurrentChunkEntityIndex = chunkEntityIndex;
}