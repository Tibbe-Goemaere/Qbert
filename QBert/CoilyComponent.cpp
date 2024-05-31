#include "CoilyComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"


dae::CoilyComponent::CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	,m_pCoilyState{ std::move(CoilyState::EggState) }
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

dae::GameObject* dae::CoilyComponent::GetParent() const
{
	return m_pParent;
}
