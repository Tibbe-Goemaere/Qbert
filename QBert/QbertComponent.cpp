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
#include "HealthDisplayComponent.h"
#include "TimeManager.h"

dae::QbertComponent::QbertComponent(dae::GameObject* pParent, dae::HealthComponent* pHealthComponent, LevelComponent* pLevel, float speed)
	:BaseComponent::BaseComponent(pParent)
	,m_pScoreComponent{ pParent->GetComponent<ScoreComponent>() }
	,m_pHealthComponent{pHealthComponent}
	,m_pDisk{nullptr}
	,m_justDropped{false}
	,m_isDead{false}
	,m_deadTimer{0.f}
	,m_maxDeadTime{2.f}
	,m_timeToRespawn{0.3f}
{
	m_pCollision = pParent->GetComponent<CollisionComponent>();
	if (m_pCollision == nullptr)
	{
		m_pCollision = pParent->AddComponent<CollisionComponent>(pLevel);
	}
	m_pCollision->GetSubject()->AddObserver(this);

	m_pRenderQbert = m_pParent->AddComponent<dae::RenderComponent>();
	m_pRenderQbert->SetTexture("Sprites/Qbert.png");
	const glm::vec2 curseOffset{-10.f,-45.f};
	m_pRenderQbert->SetTexture("Sprites/Curse.png", false, curseOffset);

	m_pMovecomponent = pParent->GetComponent<MoveComponent>();
	if (m_pMovecomponent == nullptr)
	{
		m_pMovecomponent = pParent->AddComponent<MoveComponent>(pLevel, EntityType::Player, 0, 0, speed);
	}
}

void dae::QbertComponent::MovePlayer(const glm::vec2& direction)
{
	if (m_pDisk != nullptr || m_isDead)
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
		Die();
		break;
	default:
		break;
	}
}

void dae::QbertComponent::Die()
{
	if (m_pHealthComponent)
	{
		m_pHealthComponent->RemoveHealth();
	}
	auto& ss = SoundLocater::GetSoundsystem();
	ss.Play("../Data/Sounds/DieSound.wav", 1);
	m_pRenderQbert->SetRenderTexture(true, 1);
	m_isDead = true;
	m_pMovecomponent->GetLevel()->KillAllEnemies();
}

void dae::QbertComponent::Respawn()
{
	m_isDead = false;
	m_deadTimer = 0.f;
	m_pRenderQbert->SetRenderTexture(true, 0);

	if (m_pMovecomponent->GetCurrentState() == MovementState::Falling)
	{
		m_pMovecomponent->ResetPosition();
	}
}


void dae::QbertComponent::Update()
{
	if (m_isDead)
	{
		m_deadTimer += TimeManager::GetInstance().GetDeltaTime();
		
		if (m_deadTimer >= m_maxDeadTime)
		{
			m_pRenderQbert->HideAllTextures();
			if (m_deadTimer >= m_maxDeadTime + m_timeToRespawn)
			{
				Respawn();
			}
		}
	}
	else if (m_pMovecomponent->GetCurrentState() == MovementState::Arriving)
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
				Die();
				m_pMovecomponent->StartFalling();
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


