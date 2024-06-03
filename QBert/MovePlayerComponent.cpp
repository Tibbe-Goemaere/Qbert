#include "MovePlayerComponent.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "DiskComponent.h"
#include <iostream>

dae::MovePlayerComponent::MovePlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed)
	:MoveComponent::MoveComponent(pParent,pLevel, speed)
	,m_pScoreComponent{ pParent->GetComponent<ScoreComponent>() }
	,m_pHealthComponent{ pParent->GetComponent<HealthComponent>() }
	,m_pDisk{nullptr}
	,m_justDropped{false}
{
	m_entityIdx = pLevel->AddEntity(std::make_unique<Entity>(m_pCurrentBlock->row, m_pCurrentBlock->column,EntityType::Player));
}

void dae::MovePlayerComponent::MovePlayer(const glm::vec2& direction)
{
	if (m_pDisk != nullptr)
	{
		return;
	}
	if (m_pCurrentBlock == nullptr)
	{
		return;
	}

	int currentRow = m_pCurrentBlock->row;
	int currentColumn = m_pCurrentBlock->column;

	if (Move(direction))
	{
		GetNextRowColumn(currentRow, currentColumn, direction);
		m_pLevel->UpdateEntity(m_entityIdx, currentRow, currentColumn);
		auto& ss = SoundLocater::GetSoundsystem();
		ss.Play("../Data/Sounds/JumpSound.wav", 1);
	}

	if (CheckDeath())
	{
		m_pDisk = GetLevel()->GetDisk(currentRow, currentColumn);
	}
}

void dae::MovePlayerComponent::GetOffDisk()
{
	m_pParent->SetParent(nullptr, true);
	m_pDisk = nullptr;
	DropOnLevel();
	m_justDropped = true;
}


void dae::MovePlayerComponent::Update()
{
	MoveComponent::Update();

	if (GetCurrentState() == MovementState::Arriving)
	{	
		if (m_pDisk != nullptr)
		{
			m_pParent->SetParent(m_pDisk->GetParent(), true);
			m_pDisk->Activate();
		}
		else
		{
			if (CheckDeath())
			{
				StartFalling();
				auto& ss = SoundLocater::GetSoundsystem();
				ss.Play("../Data/Sounds/DieSound.wav", 1);
				if (m_pHealthComponent)
				{
					m_pHealthComponent->RemoveHealth();
				}
				return;
			}

			if (m_justDropped)
			{
				m_pLevel->UpdateEntity(m_entityIdx, m_pCurrentBlock->row, m_pCurrentBlock->column);
				m_justDropped = false;
			}
			
			if (m_pLevel->ChangeBlock(m_pCurrentBlock->idx, m_pCurrentBlock->textureIndex) && m_pScoreComponent)
			{
				const int colorChangePoints = 25;
				m_pScoreComponent->AddScore(colorChangePoints);
			}
		}
	}
}


