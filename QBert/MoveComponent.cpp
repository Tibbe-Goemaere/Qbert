#include "MoveComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CostumCommands.h"

dae::MoveComponent::MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevel{ pLevel }
	,m_currentState{MovementState::Idle}
	,m_targetPosition{ glm::vec3{} }
	,m_speed{speed}
{
	auto pos = pLevel->GetParent()->GetLocalPosition();
	m_blockSize = pLevel->GetBlockSize();
	auto qbertSize = pParent->GetComponent<dae::RenderComponent>()->GetTextureSize();

	pParent->SetLocalPosition(glm::vec3(pos.x + m_blockSize /2.f - qbertSize.x/2.f,pos.y - qbertSize.y/2.f,pos.z));

	m_pCurrentBlock = pLevel->GetBlock(0,0);
}

bool dae::MoveComponent::Move(const dae::Direction& direction)
{
	if (!(m_currentState == MovementState::Idle))
	{
		return false;
	}

	auto pos = m_pParent->GetLocalPosition();
	
	switch (direction)
	{
	case Direction::Topleft:
	{
		m_targetPosition = { pos.x - m_blockSize / 2.f,pos.y - m_blockSize * 3.f / 4.f,pos.z };
		m_pCurrentBlock = m_pLevel->GetBlock(m_pCurrentBlock->row - 1 ,m_pCurrentBlock->column - 1);
		
		break;
	}
	case Direction::TopRight:
	{
		m_targetPosition = { pos.x + m_blockSize / 2.f,pos.y - m_blockSize * 3.f / 4.f,pos.z };
		m_pCurrentBlock = m_pLevel->GetBlock(m_pCurrentBlock->row - 1,m_pCurrentBlock->column);
		break;
	}
	case Direction::BottomLeft:
	{
		m_targetPosition = { pos.x - m_blockSize / 2.f,pos.y + m_blockSize * 3.f / 4.f,pos.z };
		m_pCurrentBlock = m_pLevel->GetBlock(m_pCurrentBlock->row + 1,m_pCurrentBlock->column);
		break;
	}
	case Direction::BottomRight:
	{
		m_targetPosition = { pos.x + m_blockSize / 2.f,pos.y + m_blockSize * 3.f / 4.f,pos.z };
		m_pCurrentBlock = m_pLevel->GetBlock(m_pCurrentBlock->row + 1,m_pCurrentBlock->column + 1);
		break;
	}
	}

	m_currentState = MovementState::Moving;
	return true;
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


