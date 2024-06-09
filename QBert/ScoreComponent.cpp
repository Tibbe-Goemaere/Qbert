#include <stdexcept>
#include <SDL_ttf.h>
#include "ScoreComponent.h"
#include "GameManager.h"

dae::ScoreComponent::ScoreComponent(GameObject* pParent, const int m_startScore)
	:m_score{ m_startScore }
	, BaseComponent::BaseComponent(pParent)
{
	m_pScoreDisplayEvent = std::make_unique<Subject>();
}

void dae::ScoreComponent::AddScore(int value)
{
	m_score += value;
	GameManager::GetInstance().SetAmountOfPoints(m_score, 0);
	m_pScoreDisplayEvent->NotifyObservers(dae::Event::ScoreChanged, m_pParent);
}

int dae::ScoreComponent::GetScore() const
{
	return m_score;
}

dae::Subject* dae::ScoreComponent::GetScoreDisplay() const
{
	return m_pScoreDisplayEvent.get();
}


