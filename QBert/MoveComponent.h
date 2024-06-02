#pragma once
#include "BaseComponent.h"
#include "LevelComponent.h"

namespace dae
{
	enum class MovementState 
	{
		Idle,
		Moving,
		Falling,
		Arriving,
		Dropping
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

		bool Move(const glm::vec2& direction, float distanceScale = 1.f);
		Block* GetCurrentBlock() const;
		dae::LevelComponent* GetLevel() const;
		bool CheckDeath();
		void StartFalling();
		void DropOnLevel();
		MovementState GetCurrentState() const;

	protected:
		void GetNextRowColumn(int& row, int& column, const glm::vec2& dir);

		dae::LevelComponent* m_pLevel;
		dae::Block* m_pCurrentBlock;
		

	private:
		MovementState m_currentState;
		float m_blockSize;
		glm::vec3 m_targetPosition;
		float m_speed;
		glm::vec3 m_startPosition;
		glm::vec3 m_dropDirection;
		std::pair<int, int> m_startGridPos;

		void Fall();
		void Drop();
	};
}
