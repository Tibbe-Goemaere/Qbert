#include "DiskComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "RenderComponent.h"

dae::DiskComponent::DiskComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, bool isOnLeftSide, int rowNr)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevelComponent{pLevel}
	,m_speed{10.f}
	,m_pRenderComponent{ pParent->GetComponent<RenderComponent>() }
{
	if (m_pRenderComponent == nullptr)
	{
		m_pRenderComponent = pParent->AddComponent<RenderComponent>();
		m_pRenderComponent->SetTexture("Sprites/Disks/Disk.png");
	}

	SetStartPosition(isOnLeftSide,rowNr);
}

void dae::DiskComponent::Update()
{
	
}

void dae::DiskComponent::SetStartPosition(bool isOnLeftSide, int rowNr)
{
	glm::vec3 diskPos{};
	auto blockSize = m_pLevelComponent->GetBlockSize();

	if (isOnLeftSide)
	{
		auto blockPos = m_pLevelComponent->GetBlock(rowNr, 0)->pos;
		blockPos.x -= blockSize / 3.f;
		blockPos.y -= blockSize / 3.f;

		diskPos = { blockPos.x,blockPos.y,0 };
	}
	else
	{
		auto blockPos = m_pLevelComponent->GetBlock(rowNr, rowNr)->pos;
		blockPos.x += blockSize * 4.f / 3.f;
		blockPos.y += blockSize / 3.f;

		diskPos = { blockPos.x,blockPos.y,0 };
	}

	auto levelPos = m_pLevelComponent->GetParent()->GetLocalPosition();
	diskPos += levelPos;
	
	m_pParent->SetLocalPosition(diskPos);
}