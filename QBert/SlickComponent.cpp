#include "SlickComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "TimeManager.h"
#include "MoveComponent.h"
#include "RenderComponent.h"

dae::SlickComponent::SlickComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, const std::string& filePath)
	:BaseComponent::BaseComponent(pParent)
	,m_timer{0.f}
	,m_waitTime{1.f}
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
		m_pMoveComponent = pParent->AddComponent<MoveComponent>(pLevel);
	}
}



void dae::SlickComponent::Update()
{
	if (m_pMoveComponent == nullptr)
		return;

	bool moveLeft = false;

	switch (m_pMoveComponent->GetCurrentState())
	{
	case MovementState::Idle:

		m_timer += TimeManager::GetInstance().GetDeltaTime();
		if (m_timer < m_waitTime)
		{
			return;
		}
		m_timer = 0;

		//Movement down pyramid
		moveLeft = (rand() % 2) == 0;

		if (moveLeft)
		{
			m_pMoveComponent->Move(glm::vec2(-1, -1));
		}
		else
		{
			m_pMoveComponent->Move(glm::vec2(1, -1));
		}
		break;
	case MovementState::Arriving:
		if (m_pMoveComponent->CheckDeath())
		{
			m_pMoveComponent->StartFalling();
		}
		else
		{
			ChangeBackBlock();
		}
		break;
	default:
		break;
	}
}

void dae::SlickComponent::ChangeBackBlock()
{
	auto currentBlock = m_pMoveComponent->GetCurrentBlock();
	m_pMoveComponent->GetLevel()->ChangeBlock(currentBlock->idx, currentBlock->textureIndex, true);
}
