#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class Font;
	class LevelComponent;
	struct SpawnInfo;
	class EnemySpawner;

	enum class GameMode
	{
		SinglePlayer,
		Coop,
		Versus
	};

	class GameManager final : public Singleton<GameManager>
	{
	public:
		void SetWindowSize(glm::vec2 windowSize);

		void LoadLevel(const std::string& filename);
		void GoToNextLevel(const int gameMode);
		int GetCurrentLevelIdx() const;

	private:
		friend class Singleton<GameManager>;

		GameManager();
		virtual ~GameManager() = default;
		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		glm::vec2 m_windowSize;

		std::shared_ptr<Font> m_pFont;
		GameMode m_currentGameMode;
		const std::string m_menuName;

		std::shared_ptr<SpawnInfo> m_pCoilySpawnInfo;
		std::shared_ptr<SpawnInfo> m_pSlickSpawnInfo;
		std::shared_ptr<SpawnInfo> m_pUggSpawnInfo;

		const int m_amountOfLevels;
		int m_currentLevelIdx;

		void MakeMenu();
		void MakeSinglePlayerLevel(int idx);
		void MakeCoopLevel(int idx);
		void MakeVersusLevel(int idx);

		//Functions for making game stuff to reuse
		void MakeQbert(LevelComponent* pLevel, Scene& scene);
		void MakeSpawns(LevelComponent* pLevel, Scene& scene);
		void MakeDisks(LevelComponent* pLevel, Scene& scene);
	};
}


