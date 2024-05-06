#pragma once
#include "GameObject.h"
#include "HealthComponent.h"
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

	class DieCommand final : public Command
	{
	public:
		DieCommand(HealthComponent* pHealth);

		void Execute() override;
	private:
		HealthComponent* m_pHealthComponent;
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


