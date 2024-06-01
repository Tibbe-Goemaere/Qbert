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

	if (m_pMoveComponent->CheckDeath())
	{
		m_pMoveComponent->StartFalling();
	}

	bool moveLeft = false;

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

	return;
}


