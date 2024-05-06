#include "FollowPlayerComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "CostumCommands.h"
#include "TimeManager.h"
#include <cstdlib>

dae::FollowPlayerComponent::FollowPlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed, float waitTime)
	:BaseComponent::BaseComponent(pParent)
	, m_speed{ speed }
	, m_timer{ 0 }
	, m_waitTime{ waitTime }
{
	auto pMoveComp = pParent->GetComponent<MoveComponent>();
	if (pMoveComp == nullptr)
	{
		pMoveComp = pParent->AddComponent<MoveComponent>(pLevel, speed);
	}
	m_pMoveComponent = pMoveComp;
}

void dae::FollowPlayerComponent::Update()
{
	BaseComponent::Update();

	bool moveLeft = false;

	switch (m_pMoveComponent->GetCurrentState())
	{
	case dae::MovementState::Idle:
		m_timer += TimeManager::GetInstance().GetDeltaTime();
		if (m_timer < m_waitTime)
		{
			return;
		}
		m_timer = 0;

		if (m_pMoveComponent->GetCurrentBlock().row == (m_pMoveComponent->GetLevel()->GetAmountOfLayers() - 1))
		{
			m_timer = m_waitTime;
		}

		if (m_pMoveComponent->CheckDeath())
		{
			m_pMoveComponent->Fall();
		}


		moveLeft = (rand() % 2) == 0;

		if (moveLeft)
		{
			m_pMoveComponent->Move(dae::Direction::BottomLeft);
		}
		else
		{
			m_pMoveComponent->Move(dae::Direction::BottomRight);
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


