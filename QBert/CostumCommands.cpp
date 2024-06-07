#include "CostumCommands.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "UIComponent.h"
#include "GameManager.h"
#include "SceneManager.h"
#include <iostream>

dae::MovePlayerCommand::MovePlayerCommand(dae::GameObject* pObject, const glm::vec2& direction)
	:m_pGameObject{pObject}
	,m_direction{direction}
{
	m_pPlayerMovement = pObject->GetComponent<dae::QbertComponent>();
}

void dae::MovePlayerCommand::Execute()
{
	if (m_pPlayerMovement)
	{
		m_pPlayerMovement->MovePlayer(m_direction);
	}
}

dae::MoveArrows::MoveArrows(bool goUp, UIComponent* pUiComponent)
	:m_goUp{goUp}
	,m_pUIComponent{ pUiComponent }
{
	
}

void dae::MoveArrows::Execute()
{
	m_pUIComponent->ChangeSelection(m_goUp);
}


dae::ChooseGameMode::ChooseGameMode(const std::vector<std::string>& sceneNames, UIComponent* pUiComponent)
	:m_sceneNames{ sceneNames }
	,m_pUIComponent{pUiComponent}
{
}

void dae::ChooseGameMode::Execute()
{
	std::cout << m_pUIComponent->GetCurrentSelection();
	SceneManager::GetInstance().PickScene(m_sceneNames[m_pUIComponent->GetCurrentSelection()]);
}

dae::DieCommand::DieCommand(HealthComponent* pHealth)
	:m_pHealthComponent{ pHealth }
{
}

void dae::DieCommand::Execute()
{
	m_pHealthComponent->RemoveHealth();
}