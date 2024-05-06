#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{

	class ScoreComponent final : public BaseComponent
	{
	public:
		ScoreComponent(GameObject* pParent);
		virtual ~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void AddScore(int value);
		int GetScore() const;
		Subject* GetScoreDisplay() const;

	private:
		int m_score;
		std::unique_ptr<Subject> m_pScoreDisplayEvent;
	};
}
