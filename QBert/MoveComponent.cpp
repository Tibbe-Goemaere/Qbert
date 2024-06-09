#include "MoveComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CostumCommands.h"
#include "QbertComponent.h"

dae::MoveComponent::MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel, EntityType eType, int row, int column, float speed)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevel{ pLevel }
	,m_currentState{MovementState::Idle}
	,m_targetPosition{ glm::vec3{} }
	,m_speed{speed}
	,m_dropDirection{}
	,m_startGridPos{std::make_pair(row,column)}
	,m_fallDirection{0,1}
	,m_dropTime{1.f}
	,m_dropTimer{0.f}
{
	m_pCurrentBlock = pLevel->GetBlock(row, column);

	auto levelPos = m_pLevel->GetParent()->GetLocalPosition();
	auto pos = glm::vec2(levelPos.x, levelPos.y) + m_pCurrentBlock->pos;

	m_blockSize = pLevel->GetBlockSize();
	auto textureSize = pParent->GetComponent<dae::RenderComponent>()->GetTextureSize();

	m_startPosition = glm::vec3(pos.x + m_blockSize / 2.f - textureSize.x / 2.f, pos.y - textureSize.y / 2.f, 0);
	pParent->SetLocalPosition(m_startPosition);

	auto newEntity = std::make_unique<dae::Entity>(row, column, eType,pParent);
	m_entityIdx = pLevel->AddEntity(std::move(newEntity));
}

bool dae::MoveComponent::Move(const glm::vec2& direction, float)
{
	if (!(m_currentState == MovementState::Idle))
	{
		return false;
	}

	auto pos = m_pParent->GetLocalPosition();
	
	int row = m_pCurrentBlock->row;
	int column = m_pCurrentBlock->column;

	glm::normalize(direction);

	//Handle movement for Ugg
	if (direction.y == 0)
	{
		column += static_cast<int>(direction.x);
		m_targetPosition = { pos.x + m_blockSize * direction.x ,pos.y ,pos.z };
	}
	else
	{
		GetNextRowColumn(row, column, direction);
		m_targetPosition = { pos.x + (m_blockSize / 2.f) * direction.x ,pos.y + (m_blockSize * 3.f / 4.f) * (direction.y * -1),pos.z };
	}

	m_pCurrentBlock = m_pLevel->GetBlock(row, column);

	m_currentState = MovementState::Moving;
	m_pLevel->DisableEntity(m_entityIdx);
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
		return true;
	}
	return false;
}

void dae::MoveComponent::StartFalling(glm::vec2 direction)
{
	m_currentState = MovementState::Falling;
	m_pParent->SetRenderLayer(-1);
	m_fallDirection = glm::normalize(glm::vec2{ direction.x,direction.y * -1 });
}

void dae::MoveComponent::DropOnLevel()
{
	m_currentState = MovementState::Dropping;
	m_dropDirection = m_startPosition - m_pParent->GetLocalPosition();
	glm::normalize(m_dropDirection);
}

dae::MovementState dae::MoveComponent::GetCurrentState() const
{
	return m_currentState;
}

void dae::MoveComponent::UpdateEntity(int row, int col)
{
	m_pLevel->UpdateEntity(m_entityIdx, row, col);
}

int dae::MoveComponent::GetEntityIdx() const
{
	return m_entityIdx;
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

void dae::MoveComponent::ResetPosition()
{
	m_pParent->SetRenderLayer(1);
	m_pParent->SetLocalPosition(m_startPosition);
	m_pCurrentBlock = m_pLevel->GetBlock(m_startGridPos.first, m_startGridPos.second);
	m_currentState = MovementState::Idle;
}

void dae::MoveComponent::Fall()
{
	const float fallSpeed = 550.f;
	auto pos = m_pParent->GetLocalPosition();
	pos += fallSpeed * dae::TimeManager::GetInstance().GetDeltaTime() * glm::vec3{ m_fallDirection.x, m_fallDirection.y, 0 };
	m_pParent->SetLocalPosition(pos);
}

void dae::MoveComponent::Drop()
{
	const float dropSpeed = 1.f;
	
	auto pos = m_pParent->GetLocalPosition();
	pos += m_dropDirection * TimeManager::GetInstance().GetDeltaTime() * dropSpeed;
	m_pParent->SetLocalPosition(pos);

	float margin = 1.f;
	if (glm::distance(pos, m_startPosition) <= margin)
	{
		m_pParent->SetLocalPosition(m_startPosition);
		m_pCurrentBlock = m_pLevel->GetBlock(m_startGridPos.first, m_startGridPos.second);
		m_currentState = MovementState::Arriving;
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
			m_currentState = MovementState::Arriving;
			m_pLevel->EnableEntity(m_entityIdx);
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
		m_dropTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_dropTimer >= m_dropTime)
		{
			auto qbert = m_pParent->GetComponent<dae::QbertComponent>();
			if (qbert == nullptr)
			{
				m_pParent->MarkForDestroy();
			}
		}
		Fall();
		break;
	case dae::MovementState::Arriving:
		m_currentState = MovementState::Idle;
		break;
	case dae::MovementState::Dropping:
		Drop();
		break;
	default:
		break;
	}
}


