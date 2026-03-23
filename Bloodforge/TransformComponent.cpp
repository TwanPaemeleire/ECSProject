#include "pch.h"
#include "TransformComponent.h"
#include "EntityManager.h"
#include <memory>

namespace Bloodforge
{
	void Bloodforge::TransformComponent::SetLocalPosition(const Vector2& localPos)
	{
		m_LocalPosition = localPos;
		SetPositionDirty();
	}

	void Bloodforge::TransformComponent::SetLocalPosition(float x, float y)
	{
		SetLocalPosition({x, y});
	}

	const Vector2& Bloodforge::TransformComponent::GetWorldPosition()
	{
		if (m_PositionIsDirty)
		{
			UpdateWorldPosition();
			m_PositionIsDirty = false;
		}
		return m_WorldPosition;
	}

	void TransformComponent::SetParent(int parentEntityId, bool keepWorldPosition)
	{
		if (IsChild(parentEntityId) || parentEntityId == OwnerEntityId || m_ParentEntityId == parentEntityId) return;
		if (parentEntityId == -1)
		{
			SetLocalPosition(GetWorldPosition());
		}
		else
		{
			if (keepWorldPosition)
			{
				Entity& parent = EntityManager::GetInstance().GetEntity(parentEntityId);
				TransformComponent* parentTransform = EntityManager::GetInstance().GetComponent<TransformComponent>(parent);
				SetLocalPosition(GetWorldPosition() - parentTransform->GetWorldPosition());
			}
			SetPositionDirty();
		}
		if (m_ParentEntityId != -1)
		{
			Entity& parent = EntityManager::GetInstance().GetEntity(m_ParentEntityId);
			TransformComponent* parentTransform = EntityManager::GetInstance().GetComponent<TransformComponent>(parent);
			parentTransform->RemoveChild(OwnerEntityId);
		}
		m_ParentEntityId = parentEntityId;
		if (m_ParentEntityId != -1)
		{
			Entity& parent = EntityManager::GetInstance().GetEntity(m_ParentEntityId);
			TransformComponent* parentTransform = EntityManager::GetInstance().GetComponent<TransformComponent>(parent);
			parentTransform->AddChild(OwnerEntityId);
		}
	}

	void Bloodforge::TransformComponent::SetPositionDirty()
	{
		m_PositionIsDirty = true;
		for (int id : m_ChildEntityIds)
		{
			Entity& child = EntityManager::GetInstance().GetEntity(id);
			TransformComponent* childTransform = EntityManager::GetInstance().GetComponent<TransformComponent>(child);
			childTransform->SetPositionDirty();
		}
	}

	void Bloodforge::TransformComponent::UpdateWorldPosition()
	{
		m_WorldPosition = m_LocalPosition;
		if (m_PositionIsDirty)
		{
			if (m_ParentEntityId == -1)
			{
				m_WorldPosition = m_LocalPosition;
			}
			else
			{
				Entity& parent = EntityManager::GetInstance().GetEntity(m_ParentEntityId);
				TransformComponent* parentTransform = EntityManager::GetInstance().GetComponent<TransformComponent>(parent);
				m_WorldPosition = Vector2(parentTransform->GetWorldPosition()) + m_LocalPosition;
			}
		}
		m_PositionIsDirty = false;
	}

	void TransformComponent::AddChild(int childEntityId)
	{
		m_ChildEntityIds.emplace_back(childEntityId);
	}

	void TransformComponent::RemoveChild(int childEntityId)
	{
		m_ChildEntityIds.erase(std::remove(m_ChildEntityIds.begin(), m_ChildEntityIds.end(), childEntityId), m_ChildEntityIds.end());
	}

	bool TransformComponent::IsChild(int childEntityId)
	{
		for (int id : m_ChildEntityIds)
		{
			if (id == childEntityId) return true;

			Entity& child = EntityManager::GetInstance().GetEntity(id);
			TransformComponent* childTransform = EntityManager::GetInstance().GetComponent<TransformComponent>(child);
			if (childTransform->IsChild(childEntityId)) return true;
		}
		return false;
	}
}