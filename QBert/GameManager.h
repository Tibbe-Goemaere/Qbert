#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace dae
{
	class Scene;
	class Font;
	class LevelComponent;

	enum class GameMode
	{
		SinglePlayer,
		Coop,
		Versus
	};

	class GameManager final
	{
	public:
		GameManager(glm::vec2 windowSize);

		virtual ~GameManager() = default;
		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		void LoadLevel(const std::string& filename);
	private:
		const glm::vec2 m_windowSize;
		std::shared_ptr<Font> m_pFont;
		GameMode m_currentGameMode;
		std::vector<std::string> m_singlePlayerLevelNames;
		std::vector<std::string> m_coopLevelNames;
		std::vector<std::string> m_versusLevelNames;

		void MakeMenu();
		void MakeSinglePlayerLevel(int idx);
		void MakeCoopLevel(int idx);
		void MakeVersusLevel(int idx);

		//Functions for making game stuff to reuse
		void MakeQbert(LevelComponent* pLevel, Scene& scene);
		void AddCoilySpawn();
		void AddSlickSpawn();
		void AddUggSpawn();
		void AddBaseLevel();
	};
}


