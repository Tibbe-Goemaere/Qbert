#pragma once
#include "MoveComponent.h"

namespace dae
{
	enum class Direction;
	class ScoreComponent;
	class HealthComponent;
	class LevelComponent;

	class MovePlayerComponent final : public MoveComponent
	{
	public:
		void Update() override;

		MovePlayerComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, float speed = 150.f);
		virtual ~MovePlayerComponent() = default;
		MovePlayerComponent(const MovePlayerComponent& other) = delete;
		MovePlayerComponent(MovePlayerComponent&& other) = delete;
		MovePlayerComponent& operator=(const MovePlayerComponent& other) = delete;
		MovePlayerComponent& operator=(MovePlayerComponent&& other) = delete;

		void MovePlayer(const glm::vec2& direction);
		void GetOffDisk();
	private:
		dae::ScoreComponent* m_pScoreComponent;
		dae::HealthComponent* m_pHealthComponent;
		int m_entityIdx;
		DiskComponent* m_pDisk;
		bool m_justDropped;
	};
}
