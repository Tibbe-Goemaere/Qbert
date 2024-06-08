#pragma once
#include <vector>
#include <memory>
#include "ObjectUpdater.h"

namespace dae
{
	class BaseComponent;
	class LevelComponent;
	class Scene;

	enum class EnemyType
	{
		Coily,
		Slick,
		Ugg,
	};

	struct SpawnInfo
	{
		EnemyType enemyType;
		float timeIntervalMin;
		float timeIntervalMax;
		float startTime;

		// Constructor to initialize the struct members
		SpawnInfo(EnemyType type, float minInterval, float maxInterval, float start)
			: enemyType(type), timeIntervalMin(minInterval), timeIntervalMax(maxInterval), startTime(start)
		{}
	};

	class EnemySpawner final : public ObjectUpdater
	{
	private:
		struct Spawn
		{
			std::shared_ptr<SpawnInfo> pSpawnInfo;
			float timeUntilNextSpawn;
			float timeSinceLastSpawn = 0.f;
			bool hasSpawnedOnce = false;
		};

	public:
		void Update() override;
		void Reset() override;

		EnemySpawner(LevelComponent* pLevel, Scene& scene);
		virtual ~EnemySpawner() = default;
		EnemySpawner(const EnemySpawner& other) = delete;
		EnemySpawner(EnemySpawner&& other) = delete;
		EnemySpawner& operator=(const EnemySpawner& other) = delete;
		EnemySpawner& operator=(EnemySpawner&& other) = delete;

		void AddSpawn(std::shared_ptr<SpawnInfo> pSpawnInfo);
		void UpdateSpawn(Spawn* pSpawn, float deltaTime);

	private:
		float m_totalTime;
		std::vector<std::unique_ptr<Spawn>> m_pSpawns;
		LevelComponent* m_pLevel;
		Scene& m_scene;

		void SpawnEnemy(EnemyType enemyType);
		float GenerateRandomFloat(float min, float max);
	};
}


