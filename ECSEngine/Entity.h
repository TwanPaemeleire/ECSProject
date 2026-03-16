#pragma once
#include <string>
#include <vector>
#include "ArchetypeIdentifierMask.h"

class EntityManager;

struct Entity
{
	Entity(int id);
	~Entity() = default;

	void SetCurrentChunk(const ArchetypeIdentifierMask& archetypeIds, int chunkIndex);
	template <typename Component>
	void AddComponent();

	int Id = -1;
	ArchetypeIdentifierMask CurrentArchetypeId;
	int CurrentChunkIndex = -1;
};