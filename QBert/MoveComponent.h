#pragma once
#include "BaseComponent.h"
#include "LevelComponent.h"

namespace dae
{
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

		MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel, float speed = 150.f, int row = 0, int column = 0);
		virtual ~MoveComponent() = default;
		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

		bool Move(const glm::vec2& direction);
		Block* GetCurrentBlock() const;
		dae::LevelComponent* GetLevel() const;

	protected:
		bool CheckDeath();
		void Fall();
		void GetNextRowColumn(int& row, int& column, const glm::vec2& dir);

		dae::LevelComponent* m_pLevel;
		dae::Block* m_pCurrentBlock;
		MovementState m_currentState;

	private:
		float m_blockSize;
		glm::vec3 m_targetPosition;
		float m_speed;
	};
}
