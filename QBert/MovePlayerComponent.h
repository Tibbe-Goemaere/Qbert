#pragma once
#include "BaseComponent.h"

namespace dae
{
	enum class Direction;
	class ScoreComponent;
	class HealthComponent;
	class MoveComponent;
	class LevelComponent;

	class MovePlayerComponent final : public BaseComponent
	{
	public:
		void Update() override;

		MovePlayerComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel);
		virtual ~MovePlayerComponent() = default;
		MovePlayerComponent(const MovePlayerComponent& other) = delete;
		MovePlayerComponent(MovePlayerComponent&& other) = delete;
		MovePlayerComponent& operator=(const MovePlayerComponent& other) = delete;
		MovePlayerComponent& operator=(MovePlayerComponent&& other) = delete;

		void Move(const dae::Direction& direction);
	private:
		dae::ScoreComponent* m_pScoreComponent;
		dae::HealthComponent* m_pHealthComponent;
		dae::MoveComponent* m_pMoveComponent;
		bool m_hasMoved;
	};
}