#include "LevelComponent.h"
#include "GameObject.h"
#include <fstream>
#include <iostream>
#include "BaseComponent.h"
#include "DiskComponent.h"

dae::LevelComponent::LevelComponent(dae::GameObject* pParent, const std::string& levelPath)
	:BaseComponent::BaseComponent(pParent)
	,m_pRenderComponent(pParent->GetComponent<dae::RenderComponent>())
	,m_amountOfLayers{2}
	,m_amountOfSteps{7}
	,m_pDisks{}
{
	//WriteLevel("Level1-0.xml", XmlLevelInfo{ false,false,"../Resources/Blocks/0-2.png", "../Resources/Blocks/0-1.png"});
	LoadLevel(levelPath);
	SetTextures();
}

dae::LevelComponent::~LevelComponent()
{

}

dae::Block* dae::LevelComponent::GetBlock(const int row, const int column)
{
	auto blockIt = std::find_if(m_pBlocks.begin(), m_pBlocks.end(), [column, row](const std::unique_ptr<Block>& block) 
		{return block->column == column && block->row == row; });

	if (blockIt != m_pBlocks.end()) 
	{
		return blockIt->get();
	}
	else 
	{
		// If block is not found, return null
		return nullptr;
	}
}

dae::GameObject* dae::LevelComponent::GetParent() const
{
	return m_pParent;
}

float dae::LevelComponent::GetBlockSize() const
{
	return m_pRenderComponent->GetTextureSize(m_texturePaths[0]).x;
}

bool dae::LevelComponent::ChangeBlock(int idx, int textureIdx)
{
	const int amountOfBlocks = static_cast<int>(m_pBlocks.size());
	//Check if were not on the final layer
	if (textureIdx < (m_amountOfLayers - 1))
	{
		m_pBlocks[idx]->textureIndex += 1;
		m_pRenderComponent->SetRenderTexture(false, idx + (textureIdx * amountOfBlocks));
		m_pRenderComponent->SetRenderTexture(true, idx + ((textureIdx + 1) * amountOfBlocks));
		return true;
	}
	return false;
}

int dae::LevelComponent::GetAmountOfLayers() const
{
	return m_amountOfSteps;
}

int dae::LevelComponent::AddEntity(std::unique_ptr<Entity> pNewEntity)
{
	m_pEntities.push_back(std::move(pNewEntity));
	return static_cast<int>(m_pEntities.size()) - 1;
}

dae::Entity* dae::LevelComponent::GetEntity(EntityType entityType) const
{
	auto it = std::find_if(m_pEntities.begin(), m_pEntities.end(),
		[entityType](const std::unique_ptr<Entity>& entity)
		{
			return entity->entityType == entityType;
		});

	if (it != m_pEntities.end())
	{
		return it->get(); 
	}
	return nullptr;
}

void dae::LevelComponent::UpdateEntity(int entityIdx, int row, int column)
{
	auto pEntity = m_pEntities[entityIdx].get();
	pEntity->row = row;
	pEntity->column = column;
}

void dae::LevelComponent::AddDisk(dae::DiskComponent* pDiskComponent)
{
	m_pDisks.push_back(pDiskComponent);
}

void dae::LevelComponent::RemoveDisk(DiskComponent* pDiskComponent)
{
	m_pDisks.erase(std::remove(m_pDisks.begin(), m_pDisks.end(), pDiskComponent), m_pDisks.end());
}

dae::DiskComponent* dae::LevelComponent::GetDisk(int row, int column)
{
	auto it = std::find_if(m_pDisks.begin(), m_pDisks.end(), [row, column](dae::DiskComponent* disk) {auto [diskRow, diskCol] = disk->GetGridPosition();  return diskRow == row && diskCol == column; });
	if (it != m_pDisks.end())
	{
		return *it;
	}
	return nullptr;
}

void dae::LevelComponent::Update()
{
	BaseComponent::Update();
}

void dae::LevelComponent::SetTextures()
{
	for (int i = 0; i < m_amountOfLayers; i++)
	{
		//Add all the textures in my render component for all possible blocks, only the first layer will display
		for (const auto& block : m_pBlocks)
		{
			m_pRenderComponent->SetTexture(m_texturePaths[i], i == 0, block->pos);
		}
	}
}

void dae::LevelComponent::LoadLevel(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Error: Unable to open file for reading\n";
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.find("<hasThreeLayers>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo.hasThreeLayers = (line.find("true") != std::string::npos);
		}
		if (line.find("<isHardLevel>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo.isHardLevel = (line.find("true") != std::string::npos);
		}
		if (line.find("<texture1>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo.startBlock = line.substr(0, line.find("<"));
		}
		if (line.find("<texture2>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo.endBlock = line.substr(0, line.find("<"));
		}
		if (line.find("<texture3>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo.intermediateBlock = line.substr(0, line.find("<"));
		}
	}

	file.close();

	m_texturePaths.push_back(m_levelInfo.startBlock);
	m_texturePaths.push_back(m_levelInfo.endBlock);

	if (m_levelInfo.hasThreeLayers)
	{
		m_texturePaths.push_back(m_levelInfo.intermediateBlock);
		m_amountOfLayers = 3;
	}

	const float widthBetween = m_pRenderComponent->GetTextureSize(m_texturePaths[0]).x;
	//Make blocks at positions with the chosen texture
	int columns = 1;
	glm::vec2 pos = {0,0};

	//Make a pyramid, start at the top and increment the amount of colums each time
	int idx = 0;
	for (int row = 0; row < m_amountOfSteps; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			if (col == 0 && row != 0)
			{
				pos = pos + glm::vec2(-widthBetween / 2.f, widthBetween * 3.f / 4.f);
			}
			auto newPos = pos + glm::vec2(col * widthBetween, 0);
			auto pBlock = std::make_unique<Block>(Block{ row, col, newPos, 0, idx });
			m_pBlocks.push_back(std::move(pBlock));
			++idx;
		}
		++columns;
	}
}

void dae::LevelComponent::WriteLevel(const std::string& filename, XmlLevelInfo info)
{
	std::ofstream file(filename);

	if (!file.is_open()) 
	{
		std::cerr << "Error: Unable to open file for writing\n";
		return;
	}

	file << "<level>\n";

	file << "    <hasThreeLayers>" << std::boolalpha << info.hasThreeLayers << "</hasThreeLayers>\n";
	file << "    <isHardLevel>" << std::boolalpha << info.isHardLevel << "</isHardLevel>\n";
	file << "    <texture1>" << info.startBlock << "</texture1>\n";
	file << "    <texture2>" << info.endBlock << "</texture2>\n";
	file << "    <texture3>" << info.intermediateBlock << "</texture3>\n";

	file << "<level>\n";

	file.close();
}



