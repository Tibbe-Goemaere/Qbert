#include "DiskComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "RenderComponent.h"

dae::DiskComponent::DiskComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, bool isOnLeftSide, int rowNr)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevelComponent{pLevel}
	,m_speed{10.f}
	,m_pRenderComponent{ pParent->GetComponent<RenderComponent>() }
	,m_isMoving{false}
	,m_targetPosition{}
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
	m_isMoving = true;
}

void dae::DiskComponent::Update()
{
	
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
}