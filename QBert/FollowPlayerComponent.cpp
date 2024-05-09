#include "FollowPlayerComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "CostumCommands.h"
#include "TimeManager.h"
#include <cstdlib>

dae::FollowPlayerComponent::FollowPlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed, float waitTime)
	:MoveComponent::MoveComponent(pParent,pLevel,speed)
	, m_timer{ 0 }
	, m_waitTime{ waitTime }
{}

void dae::FollowPlayerComponent::Update()
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

		if (m_pCurrentBlock->row == (m_pLevel->GetAmountOfLayers() - 1))
		{
			m_timer = m_waitTime;
		}

		if (CheckDeath())
		{
			Fall();
		}


		moveLeft = (rand() % 2) == 0;

		if (moveLeft)
		{
			Move(FindNextBlock());
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

dae::Direction dae::FollowPlayerComponent::FindNextBlock()
{
	//m_pLevel.
	return dae::Direction();
}


