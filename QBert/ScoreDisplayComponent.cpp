#include "ScoreDisplayComponent.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "TextComponent.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* pObject, TextComponent* pText)
	:m_pointsText{ "Score " }
	, BaseComponent::BaseComponent(pObject)
{
	m_pTextComponent = pText;
	m_pScoreComponent = pObject->GetComponent<ScoreComponent>();
	const int score = m_pScoreComponent->GetScore();
	m_pTextComponent->SetText(m_pointsText + std::to_string(score));
	m_pScoreComponent->GetScoreDisplay()->AddObserver(this);
}

void dae::ScoreDisplayComponent::Notify(Event e, const GameObject*)
{
	switch (e)
	{
	case dae::Event::PlayerDies:
		break;
	case dae::Event::ScoreChanged:
		UpdatePointsText(m_pScoreComponent->GetScore());
		break;
	default:
		break;
	}
}

void dae::ScoreDisplayComponent::UpdatePointsText(const int points)
{
	m_pTextComponent->SetText(m_pointsText + std::to_string(points));
}
