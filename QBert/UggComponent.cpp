#include "UggComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "TimeManager.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include <random>
#include "CollisionComponent.h"
#include "Observer.h"

dae::UggComponent::UggComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	, m_timer{ 0.f }
	, m_waitTime{ 1.f }
	,m_lastDirection{0,0}
{
	m_pCollision = pParent->GetComponent<CollisionComponent>();
	if (m_pCollision == nullptr)
	{
		m_pCollision = pParent->AddComponent<CollisionComponent>(pLevel);
	}


	m_isUgg = (rand() % 2) == 0;
	std::string filepath = "Sprites/Wrongway.png";
	if (m_isUgg)
	{
		filepath = "Sprites/Ugg.png";
	}


	auto renderComp = pParent->GetComponent<RenderComponent>();
	if (renderComp == nullptr)
	{
		renderComp = pParent->AddComponent<RenderComponent>();
		renderComp->SetTexture(filepath);
	}

	m_pMoveComponent = pParent->GetComponent<MoveComponent>();
	if (m_pMoveComponent == nullptr)
	{
		int row = pLevel->GetAmountOfSteps() - 1;
		int col = 0;
		if (m_isUgg)
		{
			col = row;
		}
		m_pMoveComponent = pParent->AddComponent<MoveComponent>(pLevel,EntityType::PurpleEnemy,row,col);
	}
}


void dae::UggComponent::Update()
{
	switch (m_pMoveComponent->GetCurrentState())
	{
	case MovementState::Idle:

		m_timer += TimeManager::GetInstance().GetDeltaTime();
		if (m_timer < m_waitTime)
		{
			return;
		}
		m_timer = 0;

		//Movement up the pyramid
		glm::vec2 direction{0, static_cast<float>(rand() % 2) };

		if (m_isUgg)
		{
			direction.x = -1;
		}
		else
		{
			direction.x = 1;
		}

		if (m_pMoveComponent->Move(direction))
		{
			m_lastDirection = direction;
		}
		break;
	case MovementState::Arriving:
		if (m_pMoveComponent->CheckDeath())
		{
			m_pMoveComponent->StartFalling(m_lastDirection);
		}
		else
		{
			m_pMoveComponent->UpdateEntity(m_pMoveComponent->GetCurrentBlock()->row, m_pMoveComponent->GetCurrentBlock()->column);
			m_pCollision->CheckCollision(m_pMoveComponent->GetEntityIdx());
		}
		break;
	default:
		break;
	}
}
