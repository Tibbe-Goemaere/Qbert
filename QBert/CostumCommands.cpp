#include "CostumCommands.h"
#include "GameObject.h"
#include "MovePlayerComponent.h"

dae::MovePlayerCommand::MovePlayerCommand(dae::GameObject* pObject, const dae::Direction& direction)
	:m_pGameObject{pObject}
	,m_direction{direction}
{
	m_pPlayerMovement = pObject->GetComponent<dae::MovePlayerComponent>();
}

void dae::MovePlayerCommand::Execute()
{
	m_pPlayerMovement->Move(m_direction);
}