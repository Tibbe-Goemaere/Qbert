#include "CoilyComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "FollowPlayerComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"


dae::CoilyComponent::CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	,m_pCoilyMovement{nullptr}
	,m_pCoilyState{ std::move(CoilyMoveState::EggState) }
{
	m_pMoveComponent = pParent->GetComponent<MoveComponent>();
	if (m_pMoveComponent == nullptr)
	{
		m_pMoveComponent = pParent->AddComponent<MoveComponent>(pLevel);
	}

	m_pRenderComponent = pParent->GetComponent<RenderComponent>();
	if (m_pRenderComponent == nullptr)
	{
		m_pRenderComponent = pParent->AddComponent<RenderComponent>();
	}

	m_pCoilyState->OnEnter(this);
}

void dae::CoilyComponent::Update()
{
	auto newState = m_pCoilyState->Update(this);
	if (newState != nullptr)
	{
		m_pCoilyState->OnExit(this);
		m_pCoilyState = std::move(newState);
		m_pCoilyState->OnEnter(this);
	}
}

void dae::CoilyComponent::SetTexture(const std::string& filepath)
{
	m_pRenderComponent->SetRenderTexture(false);
	m_pRenderComponent->SetTexture(filepath);
}

void dae::CoilyComponent::ChangeToSnake()
{
	SetTexture("../Resources/Sprites/Coily.png");
	auto pLevel = m_pMoveComponent->GetLevel();
	auto pCurrentBlock = m_pMoveComponent->GetCurrentBlock();
	m_pCoilyMovement = m_pParent->AddComponent<FollowPlayerComponent>(pLevel, pCurrentBlock->row, pCurrentBlock->column);
}

bool dae::CoilyComponent::IsDead()
{
	if (m_pCoilyMovement != nullptr)
	{
		return m_pCoilyMovement->CheckDeath();
	}
	return true;
}

dae::GameObject* dae::CoilyComponent::GetParent() const
{
	return m_pParent;
}
