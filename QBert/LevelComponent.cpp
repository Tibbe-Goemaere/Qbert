#include "LevelComponent.h"
#include "GameObject.h"
#include <fstream>
#include <iostream>
#include "BaseComponent.h"
#include "DiskComponent.h"
#include <algorithm>
#include "TimeManager.h"
#include "SceneManager.h"
#include "GameManager.h"

dae::LevelComponent::LevelComponent(dae::GameObject* pParent, const std::string& levelPath)
	:BaseComponent::BaseComponent(pParent)
	,m_pRenderComponent(pParent->GetComponent<dae::RenderComponent>())
	,m_amountOfLayers{2}
	,m_amountOfSteps{7}
	,m_pDisks{}
	,m_levelInfo{std::make_unique<XmlLevelInfo>()}
	,m_levelFlickerTime{0.1f}
	,m_flickerTimer{0.f}
	,m_isFlickering{false}
	,m_totalFlickerTime{3.f}
	,m_totalFlickerTimer{0.f}
{
	//WriteLevel("../Data/Levels/Level1-1.xml", XmlLevelInfo{ false,false,0,true,false,false,"../Data/Blocks/0-2.png", "../Data/Blocks/0-1.png"});
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

bool dae::LevelComponent::ChangeBlock(int idx, int textureIdx, bool goBack)
{
	const int amountOfBlocks = static_cast<int>(m_pBlocks.size());
	//Check if were not on the final layer
	if (textureIdx < (m_amountOfLayers - 1) && !goBack)
	{
		m_pBlocks[idx]->textureIndex += 1;
		m_pRenderComponent->SetRenderTexture(false, idx + (textureIdx * amountOfBlocks));
		m_pRenderComponent->SetRenderTexture(true, idx + ((textureIdx + 1) * amountOfBlocks));
		CheckWin();
		return true;

	} //Check if we are not on the first layer
	else if (textureIdx > 0 && goBack)
	{
		m_pBlocks[idx]->textureIndex -= 1;
		m_pRenderComponent->SetRenderTexture(false, idx + (textureIdx * amountOfBlocks));
		m_pRenderComponent->SetRenderTexture(true, idx + ((textureIdx - 1) * amountOfBlocks));
		return true;
	}
	return false;
}

int dae::LevelComponent::GetAmountOfSteps() const
{
	return m_amountOfSteps;
}

bool dae::LevelComponent::CheckWin()
{
	auto lastTextureIdx = m_amountOfLayers - 1;
	bool hasWon = std::all_of(m_pBlocks.begin(), m_pBlocks.end(),
		[lastTextureIdx](const std::unique_ptr<Block>& block) {
			return block->textureIndex == lastTextureIdx;
		});
	if (hasWon)
	{
		m_isFlickering = true;
		return true;
	}
	return false;
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

dae::Entity* dae::LevelComponent::GetEntityByIdx(int entityIdx) const
{
	return m_pEntities[entityIdx].get();
}

void dae::LevelComponent::UpdateEntity(int entityIdx, int row, int column)
{
	auto pEntity = m_pEntities[entityIdx].get();
	pEntity->row = row;
	pEntity->column = column;
}

std::vector<dae::Entity*> dae::LevelComponent::LookForEntities(int entityIdx)
{
	auto pEntity = m_pEntities[entityIdx].get();
	int row = pEntity->row;
	int col = pEntity->column;

	std::vector<Entity*> m_pOtherEntitiesOnBlock;

	for (const auto& entity : m_pEntities)
	{
		if (entity.get() != pEntity && entity->row == row && entity->column == col)
		{
			m_pOtherEntitiesOnBlock.push_back(entity.get());
		}
	}

	return m_pOtherEntitiesOnBlock;
}

void dae::LevelComponent::KillAllEnemies()
{
	for (const auto& entity : m_pEntities)
	{
		if (entity->entityType != EntityType::Player)
		{
			entity->pObject->MarkForDestroy();
		}
	}
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

dae::XmlLevelInfo* dae::LevelComponent::GetLevelInfo()
{
	return m_levelInfo.get();
}

void dae::LevelComponent::Update()
{
	BaseComponent::Update();
	if (m_isFlickering)
	{
		m_flickerTimer += TimeManager::GetInstance().GetDeltaTime();
		m_totalFlickerTimer += m_flickerTimer;
		if (m_flickerTimer >= m_levelFlickerTime)
		{
			m_flickerTimer = 0;
			for (auto& block : m_pBlocks)
			{
				if (block->textureIndex >= m_amountOfLayers - 1)
				{
					ChangeBlock(block->idx, block->textureIndex, true);
					
				}
				else
				{
					ChangeBlock(block->idx, block->textureIndex);
				}
				
			}
		}

		if (m_totalFlickerTimer >= m_totalFlickerTime)
		{
			m_isFlickering = false;
			GameManager::GetInstance().GoToNextLevel(m_levelInfo->gameMode);
			//Go to next scene
		}
	}
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
			m_levelInfo->hasThreeLayers = (line.find("true") != std::string::npos);
		}
		else if (line.find("<isHardLevel>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo->isHardLevel = (line.find("true") != std::string::npos);
		}
		else if (line.find("<gameMode>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo->gameMode = std::stoi(line);
		}
		else if (line.find("<hasCoily>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo->hasCoily = (line.find("true") != std::string::npos);
		}
		else if (line.find("<hasSlick>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo->hasSlick = (line.find("true") != std::string::npos);
		}
		else if (line.find("<hasUgg>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo->hasUgg = (line.find("true") != std::string::npos);
		}
		else if (line.find("<blockIdx>") != std::string::npos) {
			line = line.substr(line.find(">") + 1);
			m_levelInfo->blockIdx = std::stoi(line);
		}
	}

	file.close();

	const std::string startString = "Blocks/" + std::to_string(m_levelInfo->blockIdx) + "-";
	const std::string endString = ".png";
	m_texturePaths.push_back(startString + "0" + endString);
	m_texturePaths.push_back(startString + "2" + endString);

	if (m_levelInfo->hasThreeLayers)
	{
		m_texturePaths.push_back(startString + "1" + endString);
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
	file << "    <gameMode>" << std::boolalpha << info.gameMode << "</gameMode>\n";
	file << "    <hasCoily>" << std::boolalpha << info.hasCoily << "</hasCoily>\n";
	file << "    <hasSlick>" << std::boolalpha << info.hasSlick << "</hasSlick>\n";
	file << "    <hasUgg>" << std::boolalpha << info.hasUgg << "</hasUgg>\n";
	file << "    <blockIdx>" << info.blockIdx << "</blockIdx>\n";

	file << "<level>\n";

	file.close();
}



