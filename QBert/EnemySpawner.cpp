#include "EnemySpawner.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include <random>
#include "CoilyComponent.h"
#include "SlickComponent.h"
#include "Scene.h"
#include "SceneManager.h"

dae::EnemySpawner::EnemySpawner(LevelComponent* pLevel, Scene& scene)
	:m_totalTime{0.f}
	,m_pSpawns{}
	,m_pLevel{pLevel}
	,m_scene{scene}
{
}

void dae::EnemySpawner::Update()
{
	auto deltaTime = TimeManager::GetInstance().GetDeltaTime();
	m_totalTime += deltaTime;
	for (auto& pSpawn : m_pSpawns)
	{
		UpdateSpawn(pSpawn.get(), deltaTime);
	}
}

void dae::EnemySpawner::AddSpawn(std::unique_ptr<SpawnInfo> pSpawnInfo)
{
	auto time = GenerateRandomFloat(pSpawnInfo->timeIntervalMin,pSpawnInfo->timeIntervalMax);
	m_pSpawns.push_back(std::make_unique<Spawn>(std::move(pSpawnInfo), time));
}

void dae::EnemySpawner::UpdateSpawn(Spawn* pSpawn, float deltaTime)
{
	auto spawnInfo = pSpawn->pSpawnInfo.get();
	if (pSpawn->hasSpawnedOnce)
	{
		pSpawn->timeSinceLastSpawn += deltaTime;
		if (pSpawn->timeSinceLastSpawn >= pSpawn->timeUntilNextSpawn)
		{
			pSpawn->timeSinceLastSpawn = 0;
			pSpawn->timeUntilNextSpawn = GenerateRandomFloat(spawnInfo->timeIntervalMin,spawnInfo->timeIntervalMax);
			SpawnEnemy(spawnInfo->enemyType);
		}
	}
	else if (m_totalTime >= spawnInfo->startTime)
	{
		SpawnEnemy(spawnInfo->enemyType);
		pSpawn->hasSpawnedOnce = true;
	}
}

void dae::EnemySpawner::SpawnEnemy(EnemyType enemyType)
{
	auto pEnemyObject = std::make_unique<dae::GameObject>();
	switch (enemyType)
	{
	case dae::EnemyType::Coily:
		pEnemyObject->AddComponent<CoilyComponent>(m_pLevel);
		break;
	case dae::EnemyType::Sam:
		pEnemyObject->AddComponent<SlickComponent>(m_pLevel,"../Data/Sprites/Sam.png");
		break;
	case dae::EnemyType::Slick:
		pEnemyObject->AddComponent<SlickComponent>(m_pLevel, "../Data/Sprites/Slick.png");
		break;
	case dae::EnemyType::Ugg:
		break;
	case dae::EnemyType::Wrongway:
		break;
	default:
		break;
	}
	m_scene.Add(std::move(pEnemyObject));
}

float dae::EnemySpawner::GenerateRandomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

