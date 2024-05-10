#include "FollowPlayerComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "CostumCommands.h"
#include "TimeManager.h"
#include <cstdlib>

dae::FollowPlayerComponent::FollowPlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel, int row, int column, float speed, float waitTime)
	:MoveComponent::MoveComponent(pParent,pLevel, speed,row,column)
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

		if (CheckDeath())
		{
			Fall();
			return;
		}

		Move(FindNextBlock());

		moveLeft = (rand() % 2) == 0;
		break;
	case dae::MovementState::Moving:
		break;
	case dae::MovementState::Falling:
		break;
	default:
		break;
	}
}

glm::vec2 dae::FollowPlayerComponent::FindNextBlock() const
{
	auto entity = m_pLevel->GetEntity(EntityType::Player);

	int rowDifference = std::abs(m_pCurrentBlock->row - entity->row) - 1;
	glm::vec2 direction(0, 0);
	if (m_pCurrentBlock->row < entity->row)
	{
		direction.y = -1;
		if (entity->column > m_pCurrentBlock->column + rowDifference)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}
	else if (m_pCurrentBlock->row > entity->row)
	{
		direction.y = 1;
		if (entity->column >= m_pCurrentBlock->column - rowDifference)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}
	else
	{
		if (m_pCurrentBlock->row == (m_pLevel->GetAmountOfLayers() - 1))
		{
			direction.y = 1;
		}
		else
		{
			direction.y = -1;
		}

		if (entity->column > m_pCurrentBlock->column)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}

	return direction;
}


