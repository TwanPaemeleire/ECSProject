#pragma once
#include <string>
#include <vector>
#include "ArchetypeIdentifierMask.h"

using TagId = unsigned int;

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
		TagId Tag = 0;
	};
}