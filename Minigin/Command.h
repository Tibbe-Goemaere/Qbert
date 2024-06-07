#pragma once
#include "GameObject.h"
#include "ScoreComponent.h"
namespace dae
{
	class Command
	{
	public:
		virtual ~Command() {}
		virtual void Execute() = 0;
	};

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* pObject, const glm::vec3& direction, const float speed);

		void Execute() override;
	private:
		GameObject* m_pGameObject;
		const glm::vec3 m_Direction;
		const float m_Speed;
	};

	
	class ScoreCommand final : public Command
	{
	public:
		ScoreCommand(ScoreComponent* pScore, const int amount);

		void Execute() override;
	private:
		ScoreComponent* m_pScoreComponent;
		const int m_addedScore;
	};
}


