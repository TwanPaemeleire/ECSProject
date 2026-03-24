#pragma once
#include <string>
#include <vector>
#include "ArchetypeIdentifierMask.h"

namespace Bloodforge
{
	struct Entity
	{
		Entity(int id);
		~Entity() = default;

		void SetCurrentChunk(const ArchetypeIdentifierMask& archetypeIds, int chunkIndex);

		int Id = -1;
		ArchetypeIdentifierMask CurrentArchetypeId;
		int CurrentChunkIndex = -1;
	};
}