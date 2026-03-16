#include "pch.h"
#include "Entity.h"

Entity::Entity(int id)
	: Id(id)
{
}

void Entity::SetCurrentChunk(const ArchetypeIdentifier& chunkId, int chunkIndex)
{
	CurrentArchetypeIds = chunkId;
	CurrentChunkIndex = chunkIndex;
}