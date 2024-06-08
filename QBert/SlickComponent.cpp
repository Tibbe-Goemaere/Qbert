#include "SlickComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "TimeManager.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"

dae::SlickComponent::SlickComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	,m_timer{0.f}
	,m_waitTime{1.f}
{
	bool isSlick = (rand() % 2) == 0;
	std::string filepath = "Sprites/Sam.png";
	if (isSlick)
	{
		filepath = "Sprites/Slick.png";
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
		m_pMoveComponent = pParent->AddComponent<MoveComponent>(pLevel,EntityType::GreenEnemy,1,static_cast<int>(isSlick));
	}

	m_pCollision = pParent->GetComponent<CollisionComponent>();
	if (m_pCollision == nullptr)
	{
		m_pCollision = pParent->AddComponent<CollisionComponent>(pLevel);
	}
	m_pCollision->GetSubject()->AddObserver(this);
}

void dae::SlickComponent::Notify(dae::Event e, const GameObject*)
{
	switch (e)
	{
	case dae::Event::PlayerDies:
		break;
	default:
		break;
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
			m_pMoveComponent->UpdateEntity(m_pMoveComponent->GetCurrentBlock()->row, m_pMoveComponent->GetCurrentBlock()->column);
			m_pCollision->CheckCollision(m_pMoveComponent->GetEntityIdx());
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
