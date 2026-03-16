#include "pch.h"
#include "Entity.h"

namespace Bloodforge
{
	Entity::Entity(int id)
		: Id(id)
	{
	}

	void Entity::SetCurrentChunk(const ArchetypeIdentifierMask& chunkId, int chunkIndex)
	{
		CurrentArchetypeId = chunkId;
		CurrentChunkIndex = chunkIndex;
	}
}