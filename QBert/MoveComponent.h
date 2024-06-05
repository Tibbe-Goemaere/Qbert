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

		MoveComponent(dae::GameObject* pParent, LevelComponent* pLevel,EntityType eType, int row = 0, int column = 0, float speed = 150.f);
		virtual ~MoveComponent() = default;
		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

		bool Move(const glm::vec2& direction, float distanceScale = 1.f);
		Block* GetCurrentBlock() const;
		dae::LevelComponent* GetLevel() const;
		bool CheckDeath();
		void StartFalling(glm::vec2 direction = {0,-1});
		void DropOnLevel();
		MovementState GetCurrentState() const;
		void UpdateEntity(int row, int col);
		int GetEntityIdx() const;
		void GetNextRowColumn(int& row, int& column, const glm::vec2& dir);

	private:
		MovementState m_currentState;
		float m_blockSize;
		glm::vec3 m_targetPosition;
		float m_speed;
		glm::vec3 m_startPosition;
		glm::vec3 m_dropDirection;
		std::pair<int, int> m_startGridPos;
		glm::vec2 m_fallDirection;
		const int m_invalidIdx = -1;
		int m_entityIdx;
		dae::LevelComponent* m_pLevel;
		dae::Block* m_pCurrentBlock;

		void Fall();
		void Drop();
	};
}
