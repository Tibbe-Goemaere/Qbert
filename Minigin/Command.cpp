#include "Command.h"
#include "TimeManager.h"

dae::MoveCommand::MoveCommand(GameObject* pObject, const glm::vec3& direction, const float speed)
	:m_pGameObject{ pObject }
	, m_Direction{ direction }
	, m_Speed{ speed }
{

}

void dae::MoveCommand::Execute()
{
	glm::vec3 newPos = m_pGameObject->GetLocalPosition() + m_Direction * m_Speed * TimeManager::GetInstance().GetDeltaTime();
	m_pGameObject->SetLocalPosition(glm::vec3(newPos.x, newPos.y,newPos.z));
}

dae::ScoreCommand::ScoreCommand(ScoreComponent* pScore, const int amount)
	:m_pScoreComponent{ pScore }
	,m_addedScore{amount}
{
}

void dae::ScoreCommand::Execute()
{
	m_pScoreComponent->AddScore(m_addedScore);
}
