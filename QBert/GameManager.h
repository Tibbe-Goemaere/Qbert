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
	class Command;
	class TextComponent;

	enum class GameMode
	{
		SinglePlayer,
		Coop,
		Versus,
		Menu
	};

	struct Highscore
	{
		std::string name;
		int score;
	};

	class GameManager final : public Singleton<GameManager>
	{
	public:
		void SetWindowSize(glm::vec2 windowSize);

		void LoadLevel(const std::string& filename);
		void GoToNextLevel();
		int GetCurrentLevelIdx() const;
		void SetAmountOfPoints(int amountOfPoints, int playerIdx = 0);

		void SetGameMode(GameMode newGameMode);

		void RemoveCoilyCommands();
		void MakeCoilyPlayer(LevelComponent* pLevel);

		void MakeLeaderboard();
		int GetCurrentLetterIdx() const;
		void SetCurrentLetterIdx(const int idx);
		void AddLetter();
		void WriteToLeaderboard();

		void MakeMenu();

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
		const std::string m_menuName;
		
		std::shared_ptr<SpawnInfo> m_pCoilySpawnInfo;
		std::shared_ptr<SpawnInfo> m_pSlickSpawnInfo;
		std::shared_ptr<SpawnInfo> m_pUggSpawnInfo;

		const int m_amountOfLevels;
		int m_currentLevelIdx;
		GameMode m_currentGameMode;

		
		void MakeSinglePlayerLevel(int idx);
		void MakeCoopLevel(int idx);
		void MakeVersusLevel(int idx);

		//Functions for making game stuff to reuse
		void MakeQbert(LevelComponent* pLevel, Scene& scene, const std::pair<int,int> gridPos = {0,0}, bool isSecondPlayer = false);
		void MakeSpawns(LevelComponent* pLevel, Scene& scene);
		void MakeDisks(LevelComponent* pLevel, Scene& scene);
		void MakeCoily(LevelComponent* pLevel, Scene& scene);

		void MakeInstructionPage(GameMode gameMode);

		std::vector<Command*> m_pCoilyCommands;
		int m_amountOfPointsPlayer1;
		std::string m_player1Name;
		int m_amountOfPointsPlayer2;

		//Leaderboard stuff
		std::vector<dae::Highscore> LoadLeaderboard();
		std::vector<dae::Highscore> m_highscores;
		const std::string m_leaderboardName;
		const std::string m_leaderboardFilePath;
		int m_currentLetterIdx;
		char ConvertIntToUpperCaseLetter(int number);
		TextComponent* m_pPlayerNameComponent;

		const std::string m_instructionName;
	};
}


