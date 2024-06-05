#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace dae
{
	class Scene;
	class Font;

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
		Scene& m_scene;
		const glm::vec2 m_windowSize;
		std::shared_ptr<Font> m_pFont;

		void MakeMenu();
	};
}


