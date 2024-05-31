#include "CoilyMoveState.h"
#include "CoilyComponent.h"
#include "MoveComponent.h"
#include "TimeManager.h"

std::unique_ptr<dae::CoilyMoveState> dae::CoilyMoveState::EggState = std::make_unique<dae::EggState>();
std::unique_ptr<dae::CoilyMoveState> dae::CoilyMoveState::SnakeState = std::make_unique<dae::SnakeState>();
std::unique_ptr<dae::CoilyMoveState> dae::CoilyMoveState::DyingState = std::make_unique<dae::DyingState>();

dae::EggState::EggState()
	:m_timer{ 0 }
	,m_waitTime{ 0.5f }
	,m_pMoveComponent{nullptr}
{
}

std::unique_ptr<dae::CoilyMoveState> dae::EggState::Update(CoilyComponent* )
{
	if (m_pMoveComponent == nullptr)
		return nullptr;

	bool moveLeft = false;

	m_timer += TimeManager::GetInstance().GetDeltaTime();
	if (m_timer < m_waitTime)
	{
		return nullptr;
	}
	m_timer = 0;

	auto pBlock = m_pMoveComponent->GetCurrentBlock();

	if (pBlock != nullptr)
	{
		if (pBlock->row == (m_pMoveComponent->GetLevel()->GetAmountOfLayers() - 1))
		{
			return std::move(CoilyMoveState::SnakeState);
		}
	}

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
		
	return nullptr;
}

void dae::EggState::OnEnter(CoilyComponent* pCoilyComponent)
{
	m_pMoveComponent = pCoilyComponent->GetParent()->GetComponent<dae::MoveComponent>();
}

std::unique_ptr<dae::CoilyMoveState> dae::SnakeState::Update(CoilyComponent* pCoilyComponent)
{
	if (pCoilyComponent->IsDead())
	{
		//return std::move(CoilyState::DyingState);
	}
	return nullptr;
}

void dae::SnakeState::OnEnter(CoilyComponent* pCoilyComponent)
{
	pCoilyComponent->ChangeToSnake();
}

std::unique_ptr<dae::CoilyMoveState> dae::DyingState::Update(CoilyComponent*)
{
	return nullptr;
}