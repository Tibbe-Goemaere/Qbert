#include "CostumCommands.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "CoilyComponent.h"
#include "UIComponent.h"
#include "GameManager.h"
#include "GameManager.h"
#include <iostream>

dae::MovePlayerCommand::MovePlayerCommand(dae::GameObject* pObject, const glm::vec2& direction)
	:m_direction{direction}
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

dae::MoveCoilyCommand::MoveCoilyCommand(dae::GameObject* pObject, const glm::vec2& direction)
	:m_direction{direction}
{
	m_pCoilyMovement = pObject->GetComponent<dae::CoilyComponent>();
}

void dae::MoveCoilyCommand::Execute()
{
	if (m_pCoilyMovement != nullptr)
	{
		m_pCoilyMovement->MoveCoily(m_direction);
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


dae::ChooseGameMode::ChooseGameMode(UIComponent* pUiComponent)
	:m_pUIComponent{pUiComponent}
{
}

void dae::ChooseGameMode::Execute()
{
	std::cout << m_pUIComponent->GetCurrentSelection();
	auto& gameManager = GameManager::GetInstance();
	gameManager.SetGameMode(static_cast<GameMode>(m_pUIComponent->GetCurrentSelection()));
	gameManager.GoToNextLevel();
}

dae::DieCommand::DieCommand(HealthComponent* pHealth)
	:m_pHealthComponent{ pHealth }
{
}

void dae::DieCommand::Execute()
{
	m_pHealthComponent->RemoveHealth();
}

dae::SkipLevelCommand::SkipLevelCommand()
{
	
}

void dae::SkipLevelCommand::Execute()
{
	GameManager::GetInstance().GoToNextLevel();
}

