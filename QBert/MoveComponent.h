#pragma once
#include "BaseComponent.h"
#include "LevelComponent.h"

namespace dae
{
	enum class Direction;
	
	enum class MovementState 
	{
		Idle,
		Moving,
		Falling
	};

	class MoveComponent final : public BaseComponent
	{
	public:
		void Update() override;

		MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed = 150.f);
		virtual ~MoveComponent() = default;
		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

		void Move(const dae::Direction& direction);
		bool CheckDeath();
		void Fall();
		MovementState GetCurrentState() const;
		dae::Block GetCurrentBlock() const;
		dae::LevelComponent* GetLevel() const;
	private:
		dae::LevelComponent* m_pLevel;
		float m_blockSize;
		dae::Block m_currentBlock;
		MovementState m_currentState;
		glm::vec3 m_targetPosition;
		float m_speed;
	};
}
