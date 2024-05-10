#include "MovePlayerComponent.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

dae::MovePlayerComponent::MovePlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed)
	:MoveComponent::MoveComponent(pParent,pLevel, speed)
	,m_pScoreComponent{ pParent->GetComponent<ScoreComponent>() }
	,m_pHealthComponent{ pParent->GetComponent<HealthComponent>() }
	,m_hasMoved{false}
{
	m_entityIdx = pLevel->AddEntity(std::make_unique<Entity>(m_pCurrentBlock->row, m_pCurrentBlock->column,EntityType::Player));
}

void dae::MovePlayerComponent::MovePlayer(const glm::vec2& direction)
{
	int currentRow = m_pCurrentBlock->row;
	int currentColumn = m_pCurrentBlock->column;

	if (Move(direction))
	{
		GetNextRowColumn(currentRow, currentColumn, direction);
		m_pLevel->UpdateEntity(m_entityIdx, currentRow, currentColumn);
		auto& ss = SoundLocater::GetSoundsystem();
		ss.Play("../Resources/Sounds/JumpSound.wav", 1);
	}
}

void dae::MovePlayerComponent::Update()
{
	MoveComponent::Update();

	switch (m_currentState)
	{
	case dae::MovementState::Idle:
		if (m_hasMoved)
		{
			if (CheckDeath())
			{
				auto& ss = SoundLocater::GetSoundsystem();
				ss.Play("../Resources/Sounds/DieSound.wav", 1);
				if (m_pHealthComponent)
				{
					m_pHealthComponent->RemoveHealth();
				}
				return;
			}

			if (m_pLevel->ChangeBlock(m_pCurrentBlock->idx, m_pCurrentBlock->textureIndex) && m_pScoreComponent)
			{
				const int colorChangePoints = 25;
				m_pScoreComponent->AddScore(colorChangePoints);
			}
			m_hasMoved = false;

			
		}
		break;
	case dae::MovementState::Moving:
		if (!m_hasMoved)
		{
			m_hasMoved = true;
		}
		break;
	case dae::MovementState::Falling:
		
		break;
	default:
		break;
	}
}


