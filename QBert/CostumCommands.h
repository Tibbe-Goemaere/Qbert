#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
	class QbertComponent;

	enum class Direction
	{
		Topleft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	class MovePlayerCommand final : public dae::Command
	{
	public:
		MovePlayerCommand(dae::GameObject* pObject, const glm::vec2& direction);

		void Execute() override;
	private:
		dae::GameObject* m_pGameObject;
		const glm::vec2 m_direction;
		dae::QbertComponent* m_pPlayerMovement;
	};
}


