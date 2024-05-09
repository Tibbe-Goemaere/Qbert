#include "MovePlayerComponent.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "MoveComponent.h"
#include "LevelComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

dae::MovePlayerComponent::MovePlayerComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed)
	:MoveComponent::MoveComponent(pParent,pLevel,speed)
	,m_pScoreComponent{ pParent->GetComponent<ScoreComponent>() }
	,m_pHealthComponent{ pParent->GetComponent<HealthComponent>() }
	,m_hasMoved{false}
{
	//pLevel->AddEntity(std::make_unique<Entity>(pMoveComp->GetCurrentBlock()->idx,EntityType::Player));
}

void dae::MovePlayerComponent::MovePlayer(const dae::Direction& direction)
{
	if (Move(direction))
	{
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


