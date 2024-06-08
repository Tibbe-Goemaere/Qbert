#include "CoilyState.h"
#include "CoilyComponent.h"
#include "MoveComponent.h"
#include "TimeManager.h"

dae::EggState::EggState()
	:m_timer{ 0 }
	,m_waitTime{ 1.f }
	,m_pMoveComponent{nullptr}
{
}

std::unique_ptr<dae::CoilyState> dae::EggState::Update(CoilyComponent* coily)
{
	if (m_pMoveComponent == nullptr)
		return nullptr;

	switch (m_pMoveComponent->GetCurrentState())
	{
	case MovementState::Idle:
	{
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
			if (pBlock->row == (m_pMoveComponent->GetLevel()->GetAmountOfSteps() - 1))
			{
				return std::move(std::make_unique<dae::SnakeState>());
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
		break;
	}
	case MovementState::Arriving:
		m_pMoveComponent->UpdateEntity(m_pMoveComponent->GetCurrentBlock()->row, m_pMoveComponent->GetCurrentBlock()->column);
		coily->CheckCollision();
		break;
	default:
		break;
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

std::unique_ptr<dae::CoilyState> dae::SnakeState::Update(CoilyComponent* pCoily)
{
	switch (m_pMoveComponent->GetCurrentState())
	{
	case MovementState::Idle:
	{
		m_timer += TimeManager::GetInstance().GetDeltaTime();
		if (m_timer < m_waitTime)
		{
			return nullptr;
		}
		m_timer = 0;

		if (m_pMoveComponent->CheckDeath())
		{
			return std::move(std::make_unique<dae::DyingState>());
		}

		m_pMoveComponent->Move(FindNextBlock());
		break;
	}
	case MovementState::Arriving:
		if (m_pMoveComponent->GetCurrentBlock())
		{
			m_pMoveComponent->UpdateEntity(m_pMoveComponent->GetCurrentBlock()->row, m_pMoveComponent->GetCurrentBlock()->column);
			pCoily->CheckCollision();
		}
		break;
	default:
		break;
	}
	return nullptr;
}

void dae::SnakeState::OnEnter(CoilyComponent* pCoilyComponent)
{
	m_pMoveComponent = pCoilyComponent->GetParent()->GetComponent<dae::MoveComponent>();
	auto pRenderComp = pCoilyComponent->GetParent()->GetComponent<dae::RenderComponent>();
	if (pRenderComp)
	{
		pRenderComp->SetTexture("Sprites/Coily.png");
	}
}

void dae::SnakeState::OnExit(CoilyComponent*)
{
	m_pMoveComponent->StartFalling();
}

glm::vec2 dae::SnakeState::FindNextBlock() const
{
	auto level = m_pMoveComponent->GetLevel();
	auto block = m_pMoveComponent->GetCurrentBlock();
	auto entity = level->GetEntity(EntityType::Player);

	float rowDifference = static_cast<float>(std::abs(block->row - entity->row)) - 0.5f;
	float myCol = static_cast<float>(block->column);
	float entityCol = static_cast<float>(entity->column);
	glm::vec2 direction(0, 1);

	if (block->row < entity->row)
	{
		//Go down
		direction.y = -1;
		//Decide horizontal movement
		if (entityCol > myCol + rowDifference)
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
		//Go up
		direction.y = 1;
		//Decide horizontal movement
		if (entityCol >= myCol - rowDifference)
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
		//If at same height just go left or right based on where you are
		if (entity->column > block->column)
		{
			direction.x = 1;
		}
		else
		{
			direction.x = -1;
		}
	}

	bool isNotInJumpDistance = std::abs(entity->row - block->row) != 1;
	//If at bottom always go up
	if (block->row == (level->GetAmountOfSteps() - 1))
	{
		direction.y = 1;
	}
	//If at top always go down
	if (block->row == 0 && isNotInJumpDistance)
	{
		direction.y = -1;
	}

	//If at the far right edge only jump off when its after chasing qbert
	if (block->row == block->column && isNotInJumpDistance)
	{
		direction.x = -1;
	}
	//If at the left side edge only jump off when its after chasing qbert
	if (0 == block->column && isNotInJumpDistance)
	{
		direction.x = 1;
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
