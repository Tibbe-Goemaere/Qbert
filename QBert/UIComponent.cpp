#include "UIComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

dae::UIComponent::UIComponent(GameObject* pParent, std::vector<RenderComponent*> pArrows)
	:BaseComponent::BaseComponent(pParent)
	,m_currentSelection{0}
	,m_pArrows{pArrows}
{
	for (int i = 1; i < m_pArrows.size(); i++)
	{
		m_pArrows[i]->HideAllTextures();
	}
}

int dae::UIComponent::GetCurrentSelection() const
{
	return m_currentSelection;
}

void dae::UIComponent::ChangeSelection(bool goUp)
{
	if (m_pArrows.size() <= 1)
	{
		return;
	}

	m_pArrows[m_currentSelection]->HideAllTextures();

	if (goUp)
	{
		if (m_currentSelection == 0)
		{
			m_currentSelection = static_cast<int>(m_pArrows.size() - 1);
		}
		else
		{
			--m_currentSelection;
		}
	}
	else
	{
		if (m_currentSelection == static_cast<int>(m_pArrows.size() - 1))
		{
			m_currentSelection = 0;
		}
		else
		{
			++m_currentSelection;
		}
	}

	m_pArrows[m_currentSelection]->ShowAllTextures();
}
