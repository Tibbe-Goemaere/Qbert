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

	class MoveComponent : public BaseComponent
	{
	public:
		virtual void Update() override;

		MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed = 150.f);
		virtual ~MoveComponent() = default;
		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

	protected:
		bool Move(const dae::Direction& direction);
		bool CheckDeath();
		void Fall();

		dae::LevelComponent* m_pLevel;
		dae::Block* m_pCurrentBlock;
		MovementState m_currentState;

	private:
		float m_blockSize;
		glm::vec3 m_targetPosition;
		float m_speed;
	};
}
