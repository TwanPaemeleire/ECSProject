#include "pch.h"
#include "Entity.h"

Entity::Entity(int id)
	: m_Id(id)
{
}

void Entity::SetCurrentChunk(const ArchetypeIdentifier& chunkId, int chunkIndex)
{
	m_CurrentArchetypeIds = chunkId;
	m_CurrentChunkIndex = chunkIndex;
}