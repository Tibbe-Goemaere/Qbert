#include "CoilyComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"
#include "CollisionComponent.h"

dae::CoilyComponent::CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel)
	:BaseComponent::BaseComponent(pParent)
	,m_pCoilyState{ std::move(std::make_unique<dae::EggState>()) }
{
	m_pRenderComponent = pParent->GetComponent<RenderComponent>();
	if (m_pRenderComponent == nullptr)
	{
		m_pRenderComponent = pParent->AddComponent<RenderComponent>();
		m_pRenderComponent->SetTexture("../Data/Sprites/CoilyEgg.png");
	}

	bool spawnLeft = (rand() % 2) == 0;
	m_pMoveComponent = pParent->GetComponent<MoveComponent>();
	if (m_pMoveComponent == nullptr)
	{
		m_pMoveComponent = pParent->AddComponent<MoveComponent>(pLevel,EntityType::PurpleEnemy,1,static_cast<int>(spawnLeft));
	}

	m_pCollision = pParent->GetComponent<CollisionComponent>();
	if (m_pCollision == nullptr)
	{
		m_pCollision = pParent->AddComponent<CollisionComponent>(pLevel);
	}
	m_pCollision->GetSubject()->AddObserver(this);

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

void dae::CoilyComponent::Notify(dae::Event e, const GameObject*)
{
	switch (e)
	{
	case dae::Event::PlayerDies:
		break;
	default:
		break;
	}
}

void dae::CoilyComponent::CheckCollision()
{
	m_pCollision->CheckCollision(m_pMoveComponent->GetEntityIdx());
}

void dae::CoilyComponent::MoveCoily(const glm::vec2 direction)
{
	if (dynamic_cast<dae::SnakeState*>(m_pCoilyState.get())) 
	{
		m_pMoveComponent->Move(direction);
	}
	
}
