#pragma once
class Entity
{
public:
	int GetId() const { return m_Id; }
private:
	int m_Id = -1;
};