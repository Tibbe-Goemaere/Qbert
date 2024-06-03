#include "UggComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "TimeManager.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include <random>

dae::UggComponent::UggComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, const std::string& filePath, bool isUgg)
	:BaseComponent::BaseComponent(pParent)
	, m_timer{ 0.f }
	, m_waitTime{ 1.f }
	,m_isUgg{isUgg}
	,m_lastDirection{0,0}
{
	auto renderComp = pParent->GetComponent<RenderComponent>();
	if (renderComp == nullptr)
	{
		renderComp = pParent->AddComponent<RenderComponent>();
		renderComp->SetTexture(filePath);
	}

	m_pMoveComponent = pParent->GetComponent<MoveComponent>();
	if (m_pMoveComponent == nullptr)
	{
		int row = pLevel->GetAmountOfLayers() - 1;
		int col = 0;
		if (m_isUgg)
		{
			col = row;
		}
		m_pMoveComponent = pParent->AddComponent<MoveComponent>(pLevel,row,col);
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
		break;
	default:
		break;
	}
}
