#pragma once
#include <vector>
#include <string>
#include "RenderComponent.h"

namespace dae
{
	class BaseComponent;
	class GameObject;
	class Texture2D;

	struct XmlLevelInfo
	{
		bool hasThreeLayers;
		bool isHardLevel;
		std::string startBlock;
		std::string endBlock;
		std::string intermediateBlock = "";
	};

	struct Block
	{
		int row;
		int column;
		glm::vec2 pos;
		int textureIndex;
		int idx;
	};

	class LevelComponent final : public BaseComponent
	{
	public:
		void Update() override;

		LevelComponent(dae::GameObject* pParent, const std::string& levelPath);
		virtual ~LevelComponent();
		LevelComponent(const LevelComponent& other) = delete;
		LevelComponent(LevelComponent&& other) = delete;
		LevelComponent& operator=(const LevelComponent& other) = delete;
		LevelComponent& operator=(LevelComponent&& other) = delete;

		std::vector<Block> GetBlocks() const;
		Block GetBlock(const int row, const int column);
		dae::GameObject* GetParent() const;
		float GetBlockSize() const;
		bool ChangeBlock(int idx, int textureIdx);
		int GetAmountOfLayers() const;

	private:
		std::vector<Block> m_blocks;
		std::vector<std::string> m_texturePaths;
		dae::RenderComponent* m_pRenderComponent;
		XmlLevelInfo m_levelInfo;
		int m_amountOfLayers;
		int m_amountOfSteps;

		void SetTextures();
		std::vector<dae::Block> LoadLevel(const std::string& filename);
		void WriteLevel(const std::string& filename, XmlLevelInfo info);
	};

	
}
