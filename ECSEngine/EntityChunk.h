#pragma once
#include <tuple>
#include <memory>
#include <stdexcept>
#include <iostream>

struct IEntityChunk
{
	virtual ~IEntityChunk() = default;
	virtual bool IsFull() const = 0;
	virtual void AddEntity(int entityId) = 0;
};

template <typename... Components> 
class EntityChunk : public IEntityChunk
{
public:
	EntityChunk(size_t capacity);
	~EntityChunk() = default;

	bool IsFull() const override { return m_IsFull; }
	void AddEntity(int entityId) override;

private:
	template<std::size_t... indexSequence>
	void ConstructComponents(std::index_sequence<indexSequence...>); // std::index_sequence is just a compile-time sequence of integers {0, 1, 2, 3, etc.}

	std::tuple<std::unique_ptr<int[]>,std::unique_ptr<Components[]>...> m_Data; // First array is for entity IDs, followed by arrays for each component type
	bool m_IsFull = false;
	int m_EntityCount = 0;
	int m_Capacity = 0;
};

template<typename ...Components>
inline EntityChunk<Components...>::EntityChunk(size_t capacity)
	: m_Capacity(capacity), m_Data(std::make_unique<int[]>(capacity),std::make_unique<Components[]>(capacity)...)
{
	std::cout << "EntityChunk created with capacity: " << capacity << std::endl;
	std::cout << "With following component types: ";
	((std::cout << typeid(Components).name() << " "), ...);
	std::cout << std::endl;
}

template<typename ...Components>
inline void EntityChunk<Components...>::AddEntity(int entityId)
{
	if (IsFull())
	{
		throw std::runtime_error("Chunk is full. Cannot add more entities.");
	}

	std::cout << "Adding entity with ID: " << entityId << " to chunk with components:";
	((std::cout << " " << typeid(Components).name()), ...);
	std::cout << std::endl << "Chunk fill progress: " << m_EntityCount + 1 << "/" << m_Capacity << std::endl;

	std::get<0>(m_Data)[m_EntityCount] = entityId;
	ConstructComponents(std::index_sequence_for<Components...>{});

	++m_EntityCount;
	m_IsFull = (m_EntityCount) >= m_Capacity;
}

template<typename ...Components>
template<std::size_t ...indexSequence>
inline void EntityChunk<Components...>::ConstructComponents(std::index_sequence<indexSequence...>)
{
	((std::get<indexSequence + 1>(m_Data)[m_EntityCount] = Components{}),...);
}