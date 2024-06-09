#include "CostumCommands.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "CoilyComponent.h"
#include "UIComponent.h"
#include "GameManager.h"
#include "GameManager.h"
#include <iostream>
#include "RenderComponent.h"

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

dae::MoveArrow::MoveArrow(bool goRight, RenderComponent* pRenderArrows)
	:m_goRight{ goRight }
	,m_pRenderArrows{ pRenderArrows }
{
}

void dae::MoveArrow::Execute()
{
	auto& gameManager = GameManager::GetInstance();
	int idx = gameManager.GetCurrentLetterIdx();
	m_pRenderArrows->SetRenderTexture(false, idx);

	if (m_goRight)
	{
		if (idx == m_pRenderArrows->GetAmountOfTextures() - 1)
		{
			idx = 0;
		}
		else
		{
			++idx;
		}
	}
	else
	{
		if (idx == 0)
		{
			idx = m_pRenderArrows->GetAmountOfTextures() - 1;
		}
		else
		{
			--idx;
		}

	}
	gameManager.SetCurrentLetterIdx(idx);
	m_pRenderArrows->SetRenderTexture(true, idx);
}

dae::ChooseLetter::ChooseLetter()
{
}

void dae::ChooseLetter::Execute()
{
	GameManager::GetInstance().AddLetter();
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

dae::EnterNameCommand::EnterNameCommand()
{
	
}

void dae::EnterNameCommand::Execute()
{
	GameManager::GetInstance().WriteToLeaderboard();
	GameManager::GetInstance().GoToNextLevel();
}
