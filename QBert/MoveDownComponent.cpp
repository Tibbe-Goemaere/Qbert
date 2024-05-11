#include "MoveDownComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "CostumCommands.h"
#include "TimeManager.h"
#include <cstdlib>

dae::MoveDownComponent::MoveDownComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed, float waitTime)
	:MoveComponent::MoveComponent(pParent,pLevel,speed)
	,m_timer{0}
	,m_waitTime{waitTime}
	,m_isAtBottom{false}
{}

bool dae::MoveDownComponent::IsAtBottom() const
{
	return m_isAtBottom;
}

dae::LevelComponent* dae::MoveDownComponent::GetLevel()
{
	return m_pLevel;
}

dae::Block* dae::MoveDownComponent::GetCurrentBlock()
{
	return m_pCurrentBlock;
}

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

		if (CheckDeath())
		{
			Fall();
			return;
		}

		if (m_pCurrentBlock != nullptr)
		{
			if (m_pCurrentBlock->row == (m_pLevel->GetAmountOfLayers() - 1))
			{
				m_isAtBottom = true;
				return;
			}
		}

		//Movement down pyramid
		moveLeft = (rand() % 2) == 0;

		if (moveLeft)
		{
			Move(glm::vec2(-1,-1));
		}
		else
		{
			Move(glm::vec2(1,-1));
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


