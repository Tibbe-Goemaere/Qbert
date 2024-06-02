#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include <memory>
#include "Scene.h"

dae::GameObject::GameObject()
	:m_renderLayer{0}
{

}

void dae::GameObject::Update()
{
	for (const auto& comp : m_pComponents)
	{
		if (comp.get())
		{
			comp->Update();
		}
	}
	for (const auto& child : m_pChildren)
	{
		if (child.get())
		{
			child->Update();
		}
	}
}

void dae::GameObject::LateUpdate()
{
	for (const auto& comp : m_pComponents)
	{
		if (comp.get())
		{
			comp->LateUpdate();
		}
	}
	for (const auto& child : m_pChildren)
	{
		if (child.get())
		{
			child->LateUpdate();
		}
	}
}

void dae::GameObject::Render() const
{
	for (const auto& comp : m_pComponents)
	{
		if (comp.get())
		{
			comp->Render();
		}
	}
	for (const auto& child : m_pChildren)
	{
		if (child.get())
		{
			child->Render();
		}
	}
}

void dae::GameObject::MarkForDestroy()
{
	m_markedForDestroy = true;
}

bool dae::GameObject::MarkedForDestroy() const
{
	return m_markedForDestroy;
}

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPos)
{
	if (pParent == this || pParent == m_pParent)
	{
		return;
	}
	if (std::find_if(m_pChildren.begin(), m_pChildren.end(), [&](const std::unique_ptr<GameObject>& ptr) 
		{ return ptr.get() == pParent; }) != m_pChildren.end())
	{
		return;
	}

	if (pParent == nullptr)
	{
		if (m_pParent == nullptr)
		{
			return;
		}
		SetLocalPosition(GetWorldPosition());
		m_pScene->Add(m_pParent->RemoveChild(this));
		m_pParent = nullptr;
	}
	else
	{
		if (m_pParent)
		{
			m_pScene->Add(pParent->RemoveChild(this));
		}

		m_pParent = pParent;
		m_pParent->AddChild(this);

		if (keepWorldPos)
		{
			SetLocalPosition(GetLocalPosition() - pParent->GetWorldPosition());
		}
	}

	SetPositionDirty();
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

dae::GameObject* dae::GameObject::GetChildAt(int idx) const
{
	return m_pChildren[idx].get();
}

int dae::GameObject::GetChildCount() const
{
	return static_cast<int>(m_pChildren.size());
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(m_pScene->GetObject(pChild));
}

std::unique_ptr<dae::GameObject> dae::GameObject::RemoveChild(GameObject* pChild)
{
	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [&](const auto& children) { return children.get() == pChild; });
	if (it != m_pChildren.end())
	{
		std::unique_ptr<GameObject> childToRemove(std::move(*it));
		m_pChildren.erase(it);
		return childToRemove;
	}
	return nullptr;
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_positionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_worldPosition;
}

void dae::GameObject::SetWorldPosition(const glm::vec3& pos)
{
	m_worldPosition.x = pos.x;
	m_worldPosition.y = pos.y;
	m_worldPosition.z = pos.z;
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetLocalPosition()
{
	if (m_positionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_localPosition;
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_localPosition.x = pos.x;
	m_localPosition.y = pos.y;
	m_localPosition.z = pos.z;
	SetPositionDirty();
}

void dae::GameObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}

void dae::GameObject::SetRenderLayer(int layer)
{
	//change this layer, if it is higher it gets rendered above other textures, all textures default to zero
	m_renderLayer = layer;
	//sort the gameobjects so the priority if okay
	m_pScene->SortObjectsRenderPriority();
}

int dae::GameObject::GetRenderLayer() const
{
	return m_renderLayer;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (m_pParent == nullptr)
		{
			m_worldPosition = m_localPosition;
		}
		else
		{
			m_worldPosition = m_pParent->GetWorldPosition() + m_localPosition;
		}
		m_positionIsDirty = false;
	}
}

void dae::GameObject::SetPositionDirty()
{
	m_positionIsDirty = true;
	for (int i{ 0 }; i < static_cast<int>(m_pChildren.size()); ++i)
	{
		m_pChildren[i]->m_positionIsDirty = true;
	}
}
