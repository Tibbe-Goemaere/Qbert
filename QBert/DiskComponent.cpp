#include "DiskComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "RenderComponent.h"
#include "TimeManager.h"
#include "QbertComponent.h"

dae::DiskComponent::DiskComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, bool isOnLeftSide, int rowNr)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevelComponent{pLevel}
	,m_speed{0.5f}
	,m_pRenderComponent{ pParent->GetComponent<RenderComponent>() }
	,m_activated{false}
	,m_targetPosition{}
	,m_direction{}
{
	if (m_pRenderComponent == nullptr)
	{
		m_pRenderComponent = pParent->AddComponent<RenderComponent>();
		m_pRenderComponent->SetTexture("Sprites/Disks/Disk.png");
	}

	SetStartPosition(isOnLeftSide,rowNr);
}

std::pair<int, int> dae::DiskComponent::GetGridPosition() const
{
	return m_gridPosition;
}

void dae::DiskComponent::Activate()
{
	m_pLevelComponent->RemoveDisk(this);
	m_activated = true;
}

dae::GameObject* dae::DiskComponent::GetParent() const
{
	return m_pParent;
}

void dae::DiskComponent::Update()
{
	if (m_activated)
	{
		auto newPos = m_pParent->GetLocalPosition() + m_direction * m_speed * dae::TimeManager::GetInstance().GetDeltaTime();
		m_pParent->SetLocalPosition(newPos);

		float margin = 1.f;

		if (glm::distance(newPos,m_targetPosition) <= margin)
		{
			m_pParent->SetLocalPosition(m_targetPosition);
			m_activated = false;
			auto qbert = m_pParent->GetChildAt(0);
			auto movement = qbert->GetComponent<dae::QbertComponent>();
			if (movement)
			{
				movement->GetOffDisk();
			}
			m_pParent->MarkForDestroy();
		}
	}
}

void dae::DiskComponent::SetStartPosition(bool isOnLeftSide, int rowNr)
{
	glm::vec3 diskPos{};
	auto blockSize = m_pLevelComponent->GetBlockSize();
	auto diskSize = m_pRenderComponent->GetTextureSize();

	if (isOnLeftSide)
	{
		m_gridPosition = std::make_pair(rowNr - 1, -1);
		auto blockPos = m_pLevelComponent->GetBlock(rowNr, 0)->pos;
		blockPos.x -= blockSize/2.f;
		blockPos.y -= blockSize;

		diskPos = { blockPos.x + diskSize.x/2.f ,blockPos.y + diskSize.y * 4.f / 3.f,0 };
	}
	else
	{
		m_gridPosition = std::make_pair(rowNr - 1, rowNr);
		auto blockPos = m_pLevelComponent->GetBlock(rowNr, rowNr)->pos;
		blockPos.x += blockSize;
		blockPos.y -= blockSize;

		diskPos = { blockPos.x - diskSize.x / 2.f,blockPos.y + diskSize.y * 4.f / 3.f, 0 };
	}

	auto levelPos = m_pLevelComponent->GetParent()->GetLocalPosition();
	diskPos += levelPos;
	
	m_pParent->SetLocalPosition(diskPos);
	m_pLevelComponent->AddDisk(this);

	//Get top position where disk should end
	auto blockPos = m_pLevelComponent->GetBlock(0, 0)->pos;
	m_targetPosition = { blockPos.x + blockSize/4.f,blockPos.y -= blockSize/3.f,0 };
	m_targetPosition += levelPos;
	//m_pParent->SetLocalPosition(m_targetPosition);

	m_direction = m_targetPosition - diskPos;
	glm::normalize(m_direction);
	
}