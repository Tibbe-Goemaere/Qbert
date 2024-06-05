#include "QbertComponent.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "DiskComponent.h"
#include <iostream>
#include "CollisionComponent.h"

dae::QbertComponent::QbertComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed)
	:BaseComponent::BaseComponent(pParent)
	,m_pScoreComponent{ pParent->GetComponent<ScoreComponent>() }
	,m_pHealthComponent{ pParent->GetComponent<HealthComponent>() }
	,m_pDisk{nullptr}
	,m_justDropped{false}
{
	m_pCollision = pParent->GetComponent<CollisionComponent>();
	if (m_pCollision == nullptr)
	{
		m_pCollision = pParent->AddComponent<CollisionComponent>(pLevel);
	}
	m_pCollision->GetSubject()->AddObserver(this);

	m_pMovecomponent = pParent->GetComponent<MoveComponent>();
	if (m_pMovecomponent == nullptr)
	{
		m_pMovecomponent = pParent->AddComponent<MoveComponent>(pLevel, EntityType::Player, 0, 0, speed);
	}
	
}

void dae::QbertComponent::MovePlayer(const glm::vec2& direction)
{
	if (m_pDisk != nullptr)
	{
		return;
	}
	auto currentBlock = m_pMovecomponent->GetCurrentBlock();
	if (currentBlock == nullptr)
	{
		return;
	}

	int currentRow = currentBlock->row;
	int currentColumn = currentBlock->column;

	if (m_pMovecomponent->Move(direction))
	{
		m_pMovecomponent->GetNextRowColumn(currentRow, currentColumn, direction);
		//Extra update for coily to follow him
		m_pMovecomponent->UpdateEntity(currentRow, currentColumn);
		auto& ss = SoundLocater::GetSoundsystem();
		ss.Play("../Data/Sounds/JumpSound.wav", 1);
	}

	if (m_pMovecomponent->CheckDeath())
	{
		m_pDisk = m_pMovecomponent->GetLevel()->GetDisk(currentRow, currentColumn);
	}
}

void dae::QbertComponent::GetOffDisk()
{
	m_pParent->SetParent(nullptr, true);
	m_pDisk = nullptr;
	m_pMovecomponent->DropOnLevel();
	m_justDropped = true;
}

void dae::QbertComponent::Notify(Event e, const GameObject* )
{
	switch (e)
	{
	case dae::Event::PlayerDies:
		if (m_pHealthComponent)
		{
			m_pHealthComponent->RemoveHealth();
		}
		break;
	default:
		break;
	}
}


void dae::QbertComponent::Update()
{
	if (m_pMovecomponent->GetCurrentState() == MovementState::Arriving)
	{	
		if (m_pDisk != nullptr)
		{
			m_pParent->SetParent(m_pDisk->GetParent(), true);
			m_pDisk->Activate();
		}
		else
		{
			if (m_pMovecomponent->CheckDeath())
			{
				m_pMovecomponent->StartFalling();
				auto& ss = SoundLocater::GetSoundsystem();
				ss.Play("../Data/Sounds/DieSound.wav", 1);
				if (m_pHealthComponent)
				{
					m_pHealthComponent->RemoveHealth();
				}
				return;
			}

			auto currentBlock = m_pMovecomponent->GetCurrentBlock();
			if (m_justDropped)
			{
				m_pMovecomponent->UpdateEntity(currentBlock->row, currentBlock->column);
				
				m_justDropped = false;
			}
			
			if (m_pMovecomponent->GetLevel()->ChangeBlock(currentBlock->idx, currentBlock->textureIndex) && m_pScoreComponent)
			{
				const int colorChangePoints = 25;
				m_pScoreComponent->AddScore(colorChangePoints);
				m_pCollision->CheckCollision(m_pMovecomponent->GetEntityIdx());
			}
		}
	}
}


