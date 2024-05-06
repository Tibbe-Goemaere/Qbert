#include "MovePlayerComponent.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

dae::MovePlayerComponent::MovePlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	, m_pScoreComponent{ pParent->GetComponent<ScoreComponent>() }
	, m_pHealthComponent{ pParent->GetComponent<HealthComponent>() }
	,m_hasMoved{false}
{
	auto pMoveComp = pParent->GetComponent<MoveComponent>();
	if (pMoveComp == nullptr)
	{
		pMoveComp = pParent->AddComponent<MoveComponent>(pLevel);
	}
	m_pMoveComponent = pMoveComp;
}

void dae::MovePlayerComponent::Move(const dae::Direction& direction)
{
	m_pMoveComponent->Move(direction);

	auto& ss = SoundLocater::GetSoundsystem();
	ss.Play("../Resources/Sounds/JumpSound.wav", 1);
}

void dae::MovePlayerComponent::Update()
{
	BaseComponent::Update();

	switch (m_pMoveComponent->GetCurrentState())
	{
	case dae::MovementState::Idle:
		if (m_hasMoved)
		{
			if (m_pMoveComponent->CheckDeath())
			{
				auto& ss = SoundLocater::GetSoundsystem();
				ss.Play("../Resources/Sounds/DieSound.wav", 1);
				if (m_pHealthComponent)
				{
					m_pHealthComponent->RemoveHealth();
				}
				return;
			}

			auto block = m_pMoveComponent->GetCurrentBlock();
			auto pLevel = m_pMoveComponent->GetLevel();

			if (pLevel->ChangeBlock(block.idx, block.textureIndex) && m_pScoreComponent)
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


