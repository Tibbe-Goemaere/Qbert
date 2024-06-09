#pragma once
#include <vector>
#include <string>
#include "RenderComponent.h"

namespace dae
{
	class BaseComponent;
	class GameObject;
	class Texture2D;
	class DiskComponent;
	class GameManager;

	struct XmlLevelInfo
	{
		bool hasThreeLayers;
		bool isHardLevel;
		int gameMode;
		bool hasCoily;
		bool hasSlick;
		bool hasUgg;
		int blockIdx;
	};

	enum class EntityType
	{
		None,
		Player,
		GreenEnemy,
		PurpleEnemy
	};

	struct Block
	{
		int row;
		int column;
		glm::vec2 pos;
		int textureIndex;
		int idx;
	};

	struct Entity
	{
		int row;
		int column;
		EntityType entityType = EntityType::None;
		GameObject* pObject;
		bool isEnabled = true;
		Entity(int row ,int col, EntityType eType, GameObject* object, bool isEnabled = true)
			:row{row}
			,column{col}
			,entityType{eType}
			,pObject{object}
			,isEnabled{isEnabled}
		{}
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

		//std::vector<Block*> GetBlocks() const;
		Block* GetBlock(const int row, const int column);
		dae::GameObject* GetParent() const;
		float GetBlockSize() const;
		bool ChangeBlock(int idx, int textureIdx, bool goBack = false);
		int GetAmountOfSteps() const;
		bool CheckWin();

		//Handling Entities
		int AddEntity(std::unique_ptr<Entity> pNewEntity);
		Entity* GetEntity(EntityType entityType) const;
		Entity* GetEntityByIdx(int  entityIdx) const;
		void UpdateEntity(int entityIdx, int row, int column);
		std::vector<dae::Entity*> LookForEntities(int entityIdx);
		void KillAllEnemies(bool saveCoily = false);
		
		void EnableEntity(int entityIdx);
		void DisableEntity(int entityIdx);


		//Handling Disks
		void AddDisk(DiskComponent* pDiskComponent);
		void RemoveDisk(DiskComponent* pDiskComponent);
		DiskComponent* GetDisk(int row, int column);
		int GetAmountOfDisks() const;

		XmlLevelInfo* GetLevelInfo();

	private:
		std::vector<std::unique_ptr<Block>> m_pBlocks;
		std::vector<std::string> m_texturePaths;
		dae::RenderComponent* m_pRenderComponent;
		std::unique_ptr<XmlLevelInfo> m_levelInfo;
		int m_amountOfLayers;
		int m_amountOfSteps;
		std::vector<std::unique_ptr<Entity>> m_pEntities;
		void KillEntities(EntityType eType, bool saveCoily);
		std::vector<DiskComponent*> m_pDisks;

		void SetTextures();
		void LoadLevel(const std::string& filename);
		void WriteLevel(const std::string& filename, XmlLevelInfo info);

		bool m_isFlickering;
		const float m_levelFlickerTime;
		float m_flickerTimer;
		const float m_totalFlickerTime;
		float m_totalFlickerTimer;
	};

	
}
