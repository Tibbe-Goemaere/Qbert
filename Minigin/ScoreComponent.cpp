#include <stdexcept>
#include <SDL_ttf.h>
#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* pParent)
	:m_score{ 0 }
	, BaseComponent::BaseComponent(pParent)
{
	m_pScoreDisplayEvent = std::make_unique<Subject>();
}

void dae::ScoreComponent::AddScore(int value)
{
	m_score += value;
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


