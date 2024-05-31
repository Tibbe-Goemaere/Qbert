#include "CoilyState.h"
#include "CoilyComponent.h"
#include "MoveComponent.h"
#include "TimeManager.h"

std::unique_ptr<dae::CoilyState> dae::CoilyState::EggState = std::make_unique<dae::EggState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::SnakeState = std::make_unique<dae::SnakeState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::DyingState = std::make_unique<dae::DyingState>();

dae::EggState::EggState()
	:m_timer{ 0 }
	,m_waitTime{ 1.f }
	,m_pMoveComponent{nullptr}
{
}

std::unique_ptr<dae::CoilyState> dae::EggState::Update(CoilyComponent* )
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
			return std::move(CoilyState::SnakeState);
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

void dae::EggState::OnExit(CoilyComponent* pCoilyComponent)
{
	auto pRenderComp = pCoilyComponent->GetParent()->GetComponent<dae::RenderComponent>();
	if (pRenderComp)
	{
		pRenderComp->SetRenderTexture(false);
	}
}


dae::SnakeState::SnakeState()
	:m_timer{ 0 }
	,m_waitTime{ 1.f }
	,m_pMoveComponent{ nullptr }
{
}

std::unique_ptr<dae::CoilyState> dae::SnakeState::Update(CoilyComponent*)
{
	m_timer += TimeManager::GetInstance().GetDeltaTime();
	if (m_timer < m_waitTime)
	{
		return nullptr;
	}
	m_timer = 0;

	if (m_pMoveComponent->CheckDeath())
	{
		return std::move(CoilyState::DyingState);
	}

	m_pMoveComponent->Move(FindNextBlock());
	return nullptr;
}

void dae::SnakeState::OnEnter(CoilyComponent* pCoilyComponent)
{
	m_pMoveComponent = pCoilyComponent->GetParent()->GetComponent<dae::MoveComponent>();
	auto pRenderComp = pCoilyComponent->GetParent()->GetComponent<dae::RenderComponent>();
	if (pRenderComp)
	{
		pRenderComp->SetTexture("../Resources/Sprites/Coily.png");
	}
}

void dae::SnakeState::OnExit(CoilyComponent*)
{
}

glm::vec2 dae::SnakeState::FindNextBlock() const
{
	auto level = m_pMoveComponent->GetLevel();
	auto block = m_pMoveComponent->GetCurrentBlock();
	auto entity = level->GetEntity(EntityType::Player);

	int rowDifference = std::abs(block->row - entity->row) - 1;
	glm::vec2 direction(0, 0);
	if (block->row < entity->row)
	{
		direction.y = -1;
		if (entity->column > block->column + rowDifference)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}
	else if (block->row > entity->row)
	{
		direction.y = 1;
		if (entity->column >= block->column - rowDifference)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}
	else
	{
		if (block->row == (level->GetAmountOfLayers() - 1))
		{
			direction.y = 1;
		}
		else
		{
			direction.y = -1;
		}

		if (entity->column > block->column)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}

	return direction;
}

dae::DyingState::DyingState()
{
}

std::unique_ptr<dae::CoilyState> dae::DyingState::Update(CoilyComponent*)
{
	return nullptr;
}

void dae::DyingState::OnEnter(CoilyComponent*)
{
}

void dae::DyingState::OnExit(CoilyComponent*)
{
}
