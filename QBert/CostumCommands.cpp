#include "CostumCommands.h"
#include "GameObject.h"
#include "QbertComponent.h"

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
