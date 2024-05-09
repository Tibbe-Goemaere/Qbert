#include "MoveDownComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "CostumCommands.h"
#include "TimeManager.h"
#include <cstdlib>

dae::MoveDownComponent::MoveDownComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed, float waitTime, bool canTransform)
	:MoveComponent::MoveComponent(pParent,pLevel,speed)
	,m_canTransform{canTransform}
	,m_timer{0}
	,m_waitTime{waitTime}
{}

void dae::MoveDownComponent::Update()
{
	MoveComponent::Update();

	bool moveLeft = false;

	switch (m_currentState)
	{
	case dae::MovementState::Idle:
		m_timer += TimeManager::GetInstance().GetDeltaTime();
		if (m_timer < m_waitTime)
		{
			return;
		}
		m_timer = 0;

		if (CheckDeath() && !m_canTransform)
		{
			Fall();
			return;
		}

		if (m_pCurrentBlock->row == (m_pLevel->GetAmountOfLayers() - 1))
		{
			m_timer = m_waitTime;
			if (m_canTransform)
			{

			}
		}

		


		moveLeft = (rand() % 2) == 0;

		if (moveLeft)
		{
			Move(dae::Direction::BottomLeft);
		}
		else
		{
			Move(dae::Direction::BottomRight);
		}
		break;
	case dae::MovementState::Moving:
		break;
	case dae::MovementState::Falling:
		break;
	default:
		break;
	}
}


