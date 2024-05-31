#include "MoveComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CostumCommands.h"

dae::MoveComponent::MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed, int row, int column)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevel{ pLevel }
	,m_currentState{MovementState::Idle}
	,m_targetPosition{ glm::vec3{} }
	,m_speed{speed}
{
	m_pCurrentBlock = pLevel->GetBlock(row, column);

	auto levelPos = m_pLevel->GetParent()->GetLocalPosition();
	auto pos = glm::vec2(levelPos.x, levelPos.y) + m_pCurrentBlock->pos;

	m_blockSize = pLevel->GetBlockSize();
	auto textureSize = pParent->GetComponent<dae::RenderComponent>()->GetTextureSize();

	pParent->SetLocalPosition(glm::vec3(pos.x + m_blockSize /2.f - textureSize.x/2.f, pos.y - textureSize.y/2.f,0));

	
}

bool dae::MoveComponent::Move(const glm::vec2& direction)
{
	if (!(m_currentState == MovementState::Idle))
	{
		return false;
	}

	auto pos = m_pParent->GetLocalPosition();
	
	int row = m_pCurrentBlock->row;
	int column = m_pCurrentBlock->column;

	GetNextRowColumn(row, column, direction);

	m_targetPosition = { pos.x + (m_blockSize / 2.f) * direction.x,pos.y + (m_blockSize * 3.f / 4.f) * (direction.y * -1),pos.z };
	m_pCurrentBlock = m_pLevel->GetBlock(row, column);

	m_currentState = MovementState::Moving;
	return true;
}

dae::Block* dae::MoveComponent::GetCurrentBlock() const
{
	return m_pCurrentBlock;
}

dae::LevelComponent* dae::MoveComponent::GetLevel() const
{
	return m_pLevel;
}

bool dae::MoveComponent::CheckDeath()
{
	if (m_pCurrentBlock == nullptr)
	{
		m_currentState = MovementState::Falling;
		m_pParent->SetRenderLayer(-1);
		return true;
	}
	return false;
}

void dae::MoveComponent::Fall()
{
	const float fallSpeed = 550.f;
	auto pos = m_pParent->GetLocalPosition();
	pos = glm::vec3(pos.x,pos.y + fallSpeed * dae::TimeManager::GetInstance().GetDeltaTime(), pos.z);
	m_pParent->SetLocalPosition(pos);
}

void dae::MoveComponent::GetNextRowColumn(int& row, int& column, const glm::vec2& dir)
{
	int dirX = static_cast<int>(dir.x);
	int dirY = static_cast<int>(dir.y);


	row += (dirY * -1);
	if (dirX == 1 && dirY == -1)
	{
		column += 1;
	}
	else if(dirX == -1 && dirY == 1)
	{
		column -= 1;
	}
}

void dae::MoveComponent::Update()
{
	BaseComponent::Update();

	float margin = 2.f;
	switch (m_currentState)
	{
	case dae::MovementState::Idle:
		break;
	case dae::MovementState::Moving:
		
		auto pos = m_pParent->GetLocalPosition();

		if (glm::distance(pos,m_targetPosition) <= margin)
		{
			m_pParent->SetLocalPosition(m_targetPosition);
			m_currentState = MovementState::Idle;
		}
		else
		{
			auto dir = m_targetPosition - pos;
			auto normalizedDir = glm::normalize(dir);
			auto newPos = pos + normalizedDir * m_speed * TimeManager::GetInstance().GetDeltaTime();
			m_pParent->SetLocalPosition(newPos);
		}
		break;
	case dae::MovementState::Falling:
		Fall();
		break;
	default:
		break;
	}
}


