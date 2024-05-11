#include "CoilyComponent.h"
#include "GameObject.h"
#include "MoveDownComponent.h"
#include "FollowPlayerComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"


dae::CoilyComponent::CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	,m_pCoilyMovement{nullptr}
	, m_pCoilyState{ std::move(CoilyState::EggState) }
{
	m_pEggMovement = pParent->GetComponent<MoveDownComponent>();
	if (m_pEggMovement == nullptr)
	{
		m_pEggMovement = pParent->AddComponent<MoveDownComponent>(pLevel);
	}

	m_pRenderComponent = pParent->GetComponent<RenderComponent>();
	if (m_pRenderComponent == nullptr)
	{
		m_pRenderComponent = pParent->AddComponent<RenderComponent>();
	}
}

void dae::CoilyComponent::Update()
{
	auto newState = m_pCoilyState->Update(this);
	if (newState != nullptr)
	{
		m_pCoilyState = std::move(newState);
		m_pCoilyState->OnEnter(this);
	}
}

void dae::CoilyComponent::SetTexture(const std::string& filepath)
{
	m_pRenderComponent->SetRenderTexture(false);
	m_pRenderComponent->SetTexture(filepath);
}

bool dae::CoilyComponent::IsAtBottom()
{
	if (m_pEggMovement != nullptr)
	{
		return m_pEggMovement->IsAtBottom();
	}
	return false;
}

void dae::CoilyComponent::ChangeToSnake()
{
	SetTexture("../Resources/Sprites/Coily.png");
	auto pLevel = m_pEggMovement->GetLevel();
	auto pCurrentBlock = m_pEggMovement->GetCurrentBlock();
	m_pParent->RemoveComponent<MoveDownComponent>();
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

