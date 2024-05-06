#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
	class MovePlayerComponent;

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
		MovePlayerCommand(dae::GameObject* pObject, const Direction& direction);

		void Execute() override;
	private:
		dae::GameObject* m_pGameObject;
		const Direction m_direction;
		dae::MovePlayerComponent* m_pPlayerMovement;
	};
}

